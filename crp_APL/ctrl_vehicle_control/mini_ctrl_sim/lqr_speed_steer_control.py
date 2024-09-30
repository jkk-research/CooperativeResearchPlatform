"""

Path tracking simulation with LQR speed and steering control

author Atsushi Sakai (@Atsushi_twi)

"""
import math
import sys
import matplotlib.pyplot as plt
import numpy as np
import scipy.linalg as la
import pathlib
from scipy.spatial.transform import Rotation
sys.path.append(str(pathlib.Path(__file__).parent.parent.parent))

# from PathPlanning.CubicSpline import cubic_spline_planner
import rclpy
from rclpy.node import Node
from nav_msgs.msg import Path
from geometry_msgs.msg import PoseStamped
from crp_msgs.msg import Ego
from autoware_auto_planning_msgs.msg import Trajectory,TrajectoryPoint
from autoware_auto_control_msgs.msg import AckermannControlCommand

# === Parameters =====

# LQR parameter
lqr_Q = np.eye(5)
lqr_R = np.eye(2)
dt = 0.1  # time tick[s]
L = 2.9  # Wheel base of the vehicle [m]
max_steer = np.deg2rad(45.0)  # maximum steering angle[rad]
lateral_error = []  # [m]

show_animation = True


def angle_mod(x, zero_2_2pi=False, degree=False):
    """
    Angle modulo operation
    Default angle modulo range is [-pi, pi)

    Parameters
    ----------
    x : float or array_like
        A angle or an array of angles. This array is flattened for
        the calculation. When an angle is provided, a float angle is returned.
    zero_2_2pi : bool, optional
        Change angle modulo range to [0, 2pi)
        Default is False.
    degree : bool, optional
        If True, then the given angles are assumed to be in degrees.
        Default is False.

    Returns
    -------
    ret : float or ndarray
        an angle or an array of modulated angle.

    Examples
    --------
    >>> angle_mod(-4.0)
    2.28318531

    >>> angle_mod([-4.0])
    np.array(2.28318531)

    >>> angle_mod([-150.0, 190.0, 350], degree=True)
    array([-150., -170.,  -10.])

    >>> angle_mod(-60.0, zero_2_2pi=True, degree=True)
    array([300.])

    """
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

    def __init__(self, x=0.0, y=0.0, yaw=0.0, v=0.0):
        self.x = x
        self.y = y
        self.yaw = yaw
        self.v = v

class LQRControl():

    def __init__(self):
        self.lqr_Q = np.eye(5)
        self.lqr_R = np.eye(2)
        self.dt = 0.1

        self.pe = 0.0
        self.pth_e = 0.0


    def pi_2_pi(self, angle):
        return angle_mod(angle)


    def solve_dare(self,A, B, Q, R):
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


    def dlqr(self,A, B, Q, R):
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


    def update(self,state, cx, cy, cyaw, ck, sp, Q, R):

        ind, e = self.calc_nearest_index(state, cx, cy, cyaw)

        lateral_error.append(e)

        tv = sp[ind]

        k = ck[ind]
        v = state.v
        th_e = self.pi_2_pi(state.yaw - cyaw[ind])

        Q[0, 0] = 10.0
        Q[1, 1] = 1.0
        Q[2, 2] = 1.0
        Q[3, 3] = 1.0
        Q[4, 4] = 1.0

        R[0, 0] = 1000.0
        R[1, 1] = 0.01


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

        K, _, _ = self.dlqr(A, B, Q, R)

        # state vector
        # x = [e, dot_e, th_e, dot_th_e, delta_v]
        # e: lateral distance to the path
        # dot_e: derivative of e
        # th_e: angle difference to the path
        # dot_th_e: derivative of th_e
        # delta_v: difference between current speed and target speed
        x = np.zeros((5, 1))
        x[0, 0] = e
        x[1, 0] = (e - self.pe) / dt
        x[2, 0] = th_e
        x[3, 0] = (th_e - self.pth_e) / dt
        x[4, 0] = v - tv

        # input vector
        # u = [delta, accel]
        # delta: steering angle
        # accel: acceleration
        ustar = -K @ x

        # calc steering input
        ff = math.atan2(L * k, 1)  # feedforward steering angle
        fb = self.pi_2_pi(ustar[0, 0])  # feedback steering angle
        delta = ff + fb

        # calc accel input
        accel = ustar[1, 0]

        return delta, ind, e, th_e, accel


    def calc_nearest_index(self,state, cx, cy, cyaw):
        dx = [state.x - icx for icx in cx]
        dy = [state.y - icy for icy in cy]

        d = [idx ** 2 + idy ** 2 for (idx, idy) in zip(dx, dy)]

        mind = min(d)

        ind = d.index(mind)

        mind = math.sqrt(mind)

        dxl = cx[ind] - state.x
        dyl = cy[ind] - state.y

        angle = self.pi_2_pi(cyaw[ind] - math.atan2(dyl, dxl))
        if angle < 0:
            mind *= -1

        return ind, mind


class ROS2Node(Node):

    def __init__(self):
        super().__init__('trajectory_subscriber')
        self.subscription = self.create_subscription(Trajectory,'/planning/scenario_planning/trajectory',self.trajectory_callback,10)
        self.ego_subsciption = self.create_subscription(Ego,'/cai/ego',self.ego_callback,10)
        self.ctrl_publisher = self.create_publisher(AckermannControlCommand,'/control/command/ctrl_cmd',10)

        self.cx = []
        self.cy = []
        self.cyaw = []
        self.ck = []
        self.speed_profile = []

        self.Controller = LQRControl()

        self.state = State()

        ctrl_cmd = AckermannControlCommand()

        ctrl_cmd.lateral.steering_tire_angle = 0.0

        self.ctrl_publisher.publish(ctrl_cmd)

    def trajectory_callback(self, msg):
        self.get_logger().info('Received trajectory')
        self.cx = [point.pose.position.x for point in msg.points]
        self.cy = [point.pose.position.y for point in msg.points]
        self.cyaw = [Rotation.from_quat([point.pose.orientation.x, point.pose.orientation.y, point.pose.orientation.z, point.pose.orientation.w]).as_euler('zyx')[0] for point in msg.points]
        
        for i in range(len(self.cx) - 1):
            dx = self.cx[i + 1] - self.cx[i]
            dy = self.cy[i + 1] - self.cy[i]
            dyaw = self.cyaw[i + 1] - self.cyaw[i]
            self.ck.append(dyaw / math.sqrt(dx**2 + dy**2))
            
        self.ck.append(self.ck[-1])  # Append the last curvature value for the last point
        
        self.speed_profile = [point.longitudinal_velocity_mps for point in msg.points]
    
    def ego_callback(self, msg):
        yaw = Rotation.from_quat([msg.pose.pose.orientation.x, msg.pose.pose.orientation.y, msg.pose.pose.orientation.z, msg.pose.pose.orientation.w]).as_euler('zyx')[0]
        self.state = State(x = msg.pose.pose.position.x, y = msg.pose.pose.position.y, yaw = yaw, v = msg.twist.twist.linear.x)

        if len(self.cx) > 0:

            delta, ind, e, th_e, accel = self.Controller.update(self.state, self.cx, self.cy, self.cyaw, self.ck, self.speed_profile, lqr_Q, lqr_R)

            control_msg = AckermannControlCommand()
            control_msg.steering_angle = delta
            control_msg.long_accel_mps2 = accel

            print(control_msg)

            self.ctrl_publisher.publish(control_msg)


def main(args=None):

    rclpy.init(args=args)
    ros_node = ROS2Node()
    rclpy.spin(ros_node)
    ros_node.destroy_node()
    rclpy.shutdown()


if __name__ == '__main__':
    main()