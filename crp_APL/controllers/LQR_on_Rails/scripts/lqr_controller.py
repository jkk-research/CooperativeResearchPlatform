#!/usr/bin/env python3

import math
import numpy as np
import scipy.linalg as la
import cubic_spline_planner
import time
from numba import jit,njit
import rclpy
from rclpy.node import Node
from autoware_control_msgs.msg import Lateral,Longitudinal
from autoware_planning_msgs.msg import Trajectory
from crp_msgs.msg import Ego

# === Parameters =====

# LQR parameter
lqr_Q = np.eye(5)
lqr_R = np.eye(2)
dt = 0.1  # time tick[s]
L = 2.9  # Wheel base of the vehicle [m]
max_steer = np.deg2rad(20.0)  # maximum steering tire angle[deg]

lqr_Q[0, 0] = 0.001
lqr_Q[1, 1] = 0.0
lqr_Q[2, 2] = 0.001
lqr_Q[3, 3] = 0.0
lqr_Q[4, 4] = 0.8

lqr_R[0, 0] = 30.0
lqr_R[1, 1] = 2.0

show_animation = True

def angle_mod(x, zero_2_2pi=False, degree=False):


    if isinstance(x, float):
        is_float = True
    else:
        is_float = False

    x = np.asarray(x).flatten()
    if degree:
        x = np.deg2rad(x)

    if zero_2_2pi:
        mod_angle = x % (2 * np.pi)
    else:
        mod_angle = (x + np.pi) % (2 * np.pi) - np.pi

    if degree:
        mod_angle = np.rad2deg(mod_angle)

    if is_float:
        return mod_angle.item()
    else:
        return mod_angle

class State:

    def __init__(self, x=0.0, y=0.0, yaw=0.0, v=0.0,steer=0.0):
        self.x = x
        self.y = y
        self.yaw = yaw
        self.v = v
        self.steer = steer

def pi_2_pi(angle):
    return angle_mod(angle)

# create an ROS controller class
class ROSController(Node):

    def __init__(self):

        super().__init__('lqr_on_rails')

        self.cx = []
        self.cy = []
        self.cyaw = []
        self.ck = []
        self.sp = []

        self.pe = 0.0
        self.pth_e = 0.0

        self.state = State(x=0.0, y=0.0, yaw=0.0, v=0.0, steer=0.0)

        # read in ros params from the launch file
        self.declare_parameter('dt', dt, rclpy.Parameter.Type.DOUBLE)
        self.declare_parameter('wheel_base', L, rclpy.Parameter.Type.DOUBLE)
        self.declare_parameter('max_steer_tire_angle', max_steer, rclpy.Parameter.Type.DOUBLE)

        dt = self.get_parameter('dt')
        L = self.get_parameter('wheel_base')
        max_steer = self.get_parameter('max_steer_tire_angle')

        self.control_clock = self.create_timer(0.05, self.control_loop)

        self.ctrl_lat_publisher = self.create_publisher(Lateral, '/control/command/control_cmdLat', 10)
        self.ctrl_long_publisher = self.create_publisher(Longitudinal, '/control/command/control_cmdLong', 10)
        self.traj_subscriber = self.create_subscription(Trajectory, '/plan/trajectory', self.recive_trajectory, 10)
        self.ego_subscriber = self.create_subscription(Ego, '/ego', self.recive_ego, 10)

        self.get_logger().info("LQR Controller Node has started!")
        
        # ros log the dt, wheel base, and max steer tire angle
        self.get_logger().info(f"Current variable value: {dt}, {L}, {max_steer}")

    
    def recive_trajectory(self, msg):

        ax = []
        ay = []

        ax = [point.pose.position.x for point in msg.points]
        ay = [point.pose.position.y for point in msg.points]
        self.sp = [point.longitudinal_velocity_mps for point in msg.points]

        self.cyaw = []    
       
        self.cyaw = np.arctan2(np.diff(ay), np.diff(ax)).tolist()

        self.cx, self.cy, yaw, self.ck, self.s = cubic_spline_planner.calc_spline_course(
            ax, ay, ds=0.2)   
        

    
    def recive_ego(self, msg):

        self.state.x = msg.pose.pose.position.x
        self.state.y = msg.pose.pose.position.y
        self.state.yaw = msg.pose.pose.orientation.z
        self.state.v = msg.twist.twist.linear.x
        self.state.steer = msg.tire_angle_front

    
    def solve_dare(self, A, B, Q, R):
        """
        solve a discrete time_Algebraic Riccati equation (DARE)
        """
        x = Q
        x_next = Q
        max_iter = 150
        eps = 0.01

        for i in range(max_iter):
            x_next = A.T @ x @ A - A.T @ x @ B @ \
                    la.inv(R + B.T @ x @ B) @ B.T @ x @ A + Q
            if (abs(x_next - x)).max() < eps:
                break
            x = x_next

        return x_next
    
    
    def dlqr(self, A, B, Q, R):
        """Solve the discrete time lqr controller.
        x[k+1] = A x[k] + B u[k]
        cost = sum x[k].T*Q*x[k] + u[k].T*R*u[k]
        # ref Bertsekas, p.151
        """

        # first, try to solve the ricatti equation
        X = self.solve_dare(A, B, Q, R)

        # compute the LQR gain
        K = la.inv(B.T @ X @ B + R) @ (B.T @ X @ A)

        eig_result = la.eig(A - B @ K)

        return K, X, eig_result[0]


    def control_loop(self):

        start_time = time.time()

        ctrl_cmd_lateral = Lateral()
        ctrl_cmd_lateral.stamp = self.get_clock().now().to_msg()
        ctrl_cmd_lateral.steering_tire_angle = 0.0
        ctrl_cmd_lateral.steering_tire_rotation_rate = 0.0

        ctrl_cmd_longitudinal = Longitudinal()
        ctrl_cmd_longitudinal.stamp = self.get_clock().now().to_msg()
        ctrl_cmd_longitudinal.velocity = 0.0
        ctrl_cmd_longitudinal.acceleration = 0.0


        if len(self.cx) <= 0:
            self.ctrl_lat_publisher.publish(ctrl_cmd_lateral)
            self.ctrl_long_publisher.publish(ctrl_cmd_longitudinal)
            return


        ind, e = self.calc_nearest_index(self.state, self.cx, self.cy, self.cyaw)


        tv = 40.0

        k = self.ck[0]
        v = self.state.v
        e = self.cy[0]
        th_e = self.cyaw[0]

        # Lateral error
        # e: lateral distance to the path
        # th_e: angle difference to the path
        # print("e: ", e)
        # print("th_e: ", th_e)
        # print(len(self.cyaw))


        # A = [1.0, dt, 0.0, 0.0, 0.0
        #      0.0, 0.0, v, 0.0, 0.0]
        #      0.0, 0.0, 1.0, dt, 0.0]
        #      0.0, 0.0, 0.0, 0.0, 0.0]
        #      0.0, 0.0, 0.0, 0.0, 1.0]
        A = np.zeros((5, 5))
        A[0, 0] = 1.0
        A[0, 1] = dt
        A[1, 2] = v
        A[2, 2] = 1.0
        A[2, 3] = dt
        A[4, 4] = 1.0

        # B = [0.0, 0.0
        #     0.0, 0.0
        #     0.0, 0.0
        #     v/L, 0.0
        #     0.0, dt]
        B = np.zeros((5, 2))
        B[3, 0] = v / L
        B[4, 1] = dt

        K, _, _ = self.dlqr(A, B, lqr_Q, lqr_R)

        # state vector
        # x = [e, dot_e, th_e, dot_th_e, delta_v]
        # e: lateral distance to the path
        # dot_e: derivative of e
        # th_e: angle difference to the path
        # dot_th_e: derivative of th_e
        # delta_v: difference between current speed and target speed
        x = np.zeros((5, 1))
        x[0, 0] = e
        x[1, 0] = 0 # (e - self.pe) / dt
        x[2, 0] = th_e
        x[3, 0] = 0 # (th_e - self.pth_e) / dt
        x[4, 0] = v - tv

        # input vector
        # u = [delta, accel]
        # delta: steering angle
        # accel: acceleration
        ustar = -K @ x

        # calc steering input
        ff = math.atan2(L * k, 1)  # feedforward steering angle
        fb = pi_2_pi(ustar[0, 0])  # feedback steering angle
        delta = ff + fb

        # calc accel input
        accel = ustar[1, 0]


        ctrl_cmd_lateral.steering_tire_angle = delta*-1
        ctrl_cmd_longitudinal.velocity = self.state.v + accel * 0.05



        self.ctrl_lat_publisher.publish(ctrl_cmd_lateral)
        self.ctrl_long_publisher.publish(ctrl_cmd_longitudinal)

        end_time = time.time()
        #print("Control loop execution time: ", end_time - start_time)


    def calc_nearest_index(self, state, cx, cy, cyaw):
        dx = [state.x - icx for icx in cx]
        dy = [state.y - icy for icy in cy]

        d = [idx ** 2 + idy ** 2 for (idx, idy) in zip(dx, dy)]

        mind = min(d)

        ind = d.index(mind)

        mind = math.sqrt(mind)

        dxl = cx[ind] - state.x
        dyl = cy[ind] - state.y

        angle = pi_2_pi(cyaw[ind] - math.atan2(dyl, dxl))
        if angle < 0:
            mind *= -1

        # Project RMS error onto vehicle
        vehicle_vector = [-np.cos(state.yaw + np.pi / 2),
                        -np.sin(state.yaw + np.pi / 2)]
        lateral_error_from_vehicle = np.dot([dx[ind], dy[ind]], vehicle_vector)

        return ind, -lateral_error_from_vehicle



def main(args=None):
    rclpy.init(args=args)

    node = ROSController()

    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass

    # Clean up and shutdown
    node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()