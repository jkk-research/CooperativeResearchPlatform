#!/usr/bin/env python3

import numpy as np
import math
import scipy.linalg as la
import time
import rclpy
from rclpy.node import Node
from autoware_control_msgs.msg import Lateral,Longitudinal
from std_msgs.msg import Float32
from autoware_planning_msgs.msg import Trajectory
from crp_msgs.msg import Ego


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

def fit_polynomial_and_curvature(x, y, degree=4):
    """
    Fits a polynomial of the specified degree to the given x, y points and calculates curvature.

    Args:
        x (array-like): The x-coordinates of the data points.
        y (array-like): The y-coordinates of the data points.
        degree (int): The degree of the polynomial to fit. Default is 3.
        plot (bool): Whether to plot the fit and curvature. Default is False.

    Returns:
        tuple:
            - numpy.ndarray: Coefficients of the polynomial in descending powers.
            - numpy.ndarray: Curvature values at the given x points.
    """
    # Fit the polynomial
    coefficients = np.polyfit(x, y, degree)
    polynomial = np.poly1d(coefficients)

    # First and second derivatives
    first_derivative = np.polyder(polynomial, 1)
    second_derivative = np.polyder(polynomial, 2)

    # Calculate curvature
    curvature = np.abs(second_derivative(x)) / (1 + first_derivative(x)**2)**(3/2)

    return curvature

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

        self.dt = 0.0333
        self.L = 2.7
        self.max_steer = np.deg2rad(20.0)

        # LQR parameter
        self.lqr_Q = np.eye(5)
        self.lqr_R = np.eye(2)

        self.lqr_Q[0, 0] = 0.001
        self.lqr_Q[1, 1] = 0.0
        self.lqr_Q[2, 2] = 0.001
        self.lqr_Q[3, 3] = 0.0
        self.lqr_Q[4, 4] = 0.8

        self.lqr_R[0, 0] = 30.0
        self.lqr_R[1, 1] = 2.0

        # read in ros params from the launch file
        self.declare_parameter('dt', self.dt)
        self.declare_parameter('wheel_base', self.L)
        self.declare_parameter('max_steer_tire_angle', self.max_steer)
        self.declare_parameter('Q', self.lqr_Q.diagonal().tolist())
        self.declare_parameter('R', self.lqr_R.diagonal().tolist())

        self.dt = self.get_parameter('dt').value
        self.L = self.get_parameter('wheel_base').value
        self.max_steer = self.get_parameter('max_steer_tire_angle').value

        self.control_clock = self.create_timer(0.05, self.control_loop)

        self.ctrl_lat_publisher = self.create_publisher(Lateral, '/control/command/control_cmdLat', 10)
        self.ctrl_long_publisher = self.create_publisher(Longitudinal, '/control/command/control_cmdLong', 10)

        self.debug_ff_publisher = self.create_publisher(Float32, '/control/lqr/debug/ff', 10)
        self.debug_fb_publisher = self.create_publisher(Float32, '/control/lqr/debug/fb', 10)

        self.traj_subscriber = self.create_subscription(Trajectory, '/plan/trajectory', self.receive_trajectory, 10)
        self.ego_subscriber = self.create_subscription(Ego, '/ego', self.receive_ego, 10)

        self.get_logger().info("LQR Controller Node has started!")
        
        # ros log the dt, wheel base, and max steer tire angle
        self.get_logger().info(f"Current variable value: {self.dt}, {self.L}, {self.max_steer}")

    
    def receive_trajectory(self, msg):

        self.cx = [point.pose.position.x for point in msg.points]
        self.cy = [point.pose.position.y for point in msg.points]
        self.sp = [point.longitudinal_velocity_mps for point in msg.points]
       
        self.cyaw = np.arctan2(np.diff(self.cy), np.diff(self.cx)).tolist()

        self.ck = fit_polynomial_and_curvature(self.cx, self.cy, degree=4)

    
    def receive_ego(self, msg):

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
        
        live_Q = self.get_parameter('Q').value
        live_R = self.get_parameter('R').value

        self.lqr_Q[0, 0] = live_Q[0]
        self.lqr_Q[1, 1] = live_Q[1]
        self.lqr_Q[2, 2] = live_Q[2]
        self.lqr_Q[3, 3] = live_Q[3]
        self.lqr_Q[4, 4] = live_Q[4]

        self.lqr_R[0, 0] = live_R[0]
        self.lqr_R[1, 1] = live_R[1]
        


        tv = self.sp[0]

        k = -self.ck[0]
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
        A[0, 1] = self.dt
        A[1, 2] = v
        A[2, 2] = 1.0
        A[2, 3] = self.dt
        A[4, 4] = 1.0

        # B = [0.0, 0.0
        #     0.0, 0.0
        #     0.0, 0.0
        #     v/L, 0.0
        #     0.0, dt]
        B = np.zeros((5, 2))
        B[3, 0] = v / self.L
        B[4, 1] = self.dt

        K, _, _ = self.dlqr(A, B, self.lqr_Q, self.lqr_R)

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
        ff = math.atan2(self.L * k, 1)  # feedforward steering angle #TODO
        fb = pi_2_pi(ustar[0, 0])  # feedback steering angle
        delta = fb + (ff*-1)

        Float32_msg = Float32()
        Float32_msg.data = ff

        self.debug_ff_publisher.publish(Float32_msg)

        Float32_msg.data = fb
        self.debug_fb_publisher.publish(Float32_msg)

        # calc accel input
        accel = ustar[1, 0]


        ctrl_cmd_lateral.steering_tire_angle = delta*-1
        ctrl_cmd_longitudinal.velocity = self.state.v + (accel * self.dt)

        self.ctrl_lat_publisher.publish(ctrl_cmd_lateral)
        self.ctrl_long_publisher.publish(ctrl_cmd_longitudinal)


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