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

import cubic_spline_planner

import rclpy
from rclpy.node import Node
from std_msgs.msg import Float32MultiArray, Float32
from nav_msgs.msg import Odometry
from crp_msgs.msg import Ego
from autoware_auto_planning_msgs.msg import Trajectory,TrajectoryPoint
from autoware_auto_control_msgs.msg import AckermannControlCommand

# === Parameters =====

dt = 0.1  # time tick[s]
L = 2.9  # Wheel base of the vehicle [m]
max_steer = np.deg2rad(45.0)  # maximum steering angle[rad]

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

def pi_2_pi(angle):
    return angle_mod(angle)

def calc_nearest_index(state, cx, cy, cyaw):

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

    return ind, mind

# function that convert euler angles to quaternions, using scipy Rotation
def euler_to_quaternion(roll, pitch, yaw):
    rot = Rotation.from_euler('xyz', [roll, pitch, yaw], degrees=False)
    rot = rot.as_quat()
    return rot


class State:

    def __init__(self, x=0.0, y=0.0, yaw=0.0, v=0.0):
        self.x = x
        self.y = y
        self.yaw = yaw
        self.v = v


def update(state, a, delta):

    if delta >= max_steer:
        delta = max_steer
    if delta <= - max_steer:
        delta = - max_steer

    state.x = state.x + state.v * math.cos(state.yaw) * dt
    state.y = state.y + state.v * math.sin(state.yaw) * dt
    state.yaw = state.yaw + state.v / L * math.tan(delta) * dt
    state.v = state.v + a * dt

    return state


# create and ROS2 class for the simulation what subscribes to the given control inputs and publishes the state of the vehicle and the desired trajectory
class ROS_Control_Sim(Node):
    def __init__(self,cx, cy, cyaw, sp):
        super().__init__('ROS_Mini_Control_Sim')

        self.control_subscriber = self.create_subscription(AckermannControlCommand, '/control/command/ctrl_cmd', self.control_input_callback, 10)
        self.traj_publisher = self.create_publisher(Trajectory, '/planning/scenario_planning/trajectory', 10)
        self.ego_publisher = self.create_publisher(Ego, '/cai/ego', 10)

        self.state = State(x=-0.0, y=-2.0, yaw=0.0, v=1.0)

        self.time = 0.0
        self.x = [self.state.x]
        self.y = [self.state.y]
        self.yaw = [self.state.yaw]
        self.v = [self.state.v]
        self.t = [0.0]

        self.traj_x = cx
        self.traj_y = cy
        self.traj_yaw = cyaw
        self.speed_profile = sp

        self.ControlCmd = AckermannControlCommand()

    def control_input_callback(self, msg):
        self.ControlCmd = msg

        self.do_simulation(self.traj_x, self.traj_y, self.traj_yaw,self.speed_profile, [self.traj_x[-1], self.traj_y[-1]])

    def create_trajectory_msg(self, x, y, yaw, v):

        # Define a rotation matrix (3x3) to rotate the point
        # This example rotates the point by 45 degrees around the z-axis
        theta = self.state.yaw
        rotation_matrix = np.array([
            [np.cos(theta), np.sin(theta), 0],
            [-np.sin(theta),  np.cos(theta), 0],
            [0,             0,             1]
        ])

        # Define a translation vector (3D)
        translation_vector = np.array([self.state.x, self.state.y, 0])

        # create a trajecotry msg transformed to the local coordinate system
        traj = Trajectory()

        target_ind, _ = calc_nearest_index(self.state, self.traj_x, self.traj_y, self.traj_yaw)

        traj.header.stamp = self.get_clock().now().to_msg()
        traj.header.frame_id = 'base_link'

        local_x = []
        local_y = []

        for i in range(target_ind, len(x)-1):

            traj_point = TrajectoryPoint()

            point_source = np.array([x[i], y[i], 0])
            point_source = point_source - translation_vector
            point_transformed = np.dot(rotation_matrix, point_source)

            traj_point.pose.position.x = point_transformed[0]
            traj_point.pose.position.y = point_transformed[1]
            traj_point.longitudinal_velocity_mps = v[i]

            local_x.append(point_transformed[0])
            local_y.append(point_transformed[1])

            dx = x[i+1] - x[i]
            dy = y[i+1] - y[i]
            heading = math.atan2(dy, dx)

            quat = euler_to_quaternion(0, 0, heading)
            traj_point.pose.orientation.x = quat[0]
            traj_point.pose.orientation.y = quat[1]
            traj_point.pose.orientation.z = quat[2]
            traj_point.pose.orientation.w = quat[3]

            traj.points.append(traj_point)

        # if target_ind % 1 == 0 and show_animation:
        #     plt.cla()
        #     # for stopping simulation with the esc key.
        #     plt.gcf().canvas.mpl_connect(
        #         'key_release_event',
        #         lambda event: [exit(0) if event.key == 'escape' else None])
        #     plt.plot(local_x,local_y, "-r", label="course")
        #     plt.axis("equal")
        #     plt.grid(True)
        #     plt.title("speed[km/h]:" + str(round(self.state.v * 3.6, 2))
        #             + ",target index:" + str(target_ind))
        #     plt.pause(0.0001)



        self.traj_publisher.publish(traj)


    def create_odom_msg(self, state):
        odometry = Odometry()

        odometry.header.stamp = self.get_clock().now().to_msg()
        odometry.header.frame_id = 'odom'
        odometry.child_frame_id = 'base_link'
        odometry.pose.pose.position.x = state.x
        odometry.pose.pose.position.y = state.y
        odometry.pose.pose.position.z = 0.0

        rot = Rotation.from_euler('xyz', [0, 0, state.yaw], degrees=False)
        rot = rot.as_quat()

        odometry.pose.pose.orientation.x = rot[0]
        odometry.pose.pose.orientation.y = rot[1]
        odometry.pose.pose.orientation.z = rot[2]
        odometry.pose.pose.orientation.w = rot[3]

        self.odometry_publisher.publish(odometry)

    def create_ego_msg(self, state):
        ego = Ego()

        ego.header.stamp = self.get_clock().now().to_msg()
        ego.header.frame_id = 'base_link'
        ego.pose.pose.position.x = state.x
        ego.pose.pose.position.y = state.y
        ego.pose.pose.position.z = 0.0

        rot = Rotation.from_euler('xyz', [0, 0, state.yaw], degrees=False)
        rot = rot.as_quat()

        ego.pose.pose.orientation.x = rot[0]
        ego.pose.pose.orientation.y = rot[1]
        ego.pose.pose.orientation.z = rot[2]
        ego.pose.pose.orientation.w = rot[3]

        self.ego_publisher.publish(ego)
    

    def do_simulation(self,cx, cy, cyaw, speed_profile, goal):
        T = 500.0  # max simulation time
        goal_dis = 0.3
        stop_speed = 0.05


        target_ind, _ = calc_nearest_index(self.state, cx, cy, cyaw)
        self.state = update(self.state, 0.0, self.ControlCmd.lateral.steering_tire_angle)

        if abs(self.state.v) <= stop_speed:
            target_ind += 1

        rot = Rotation.from_euler('xyz', [0, 0, self.state.yaw], degrees=False)
        rot = rot.as_quat()

        self.create_ego_msg(self.state)
        self.create_trajectory_msg(self.traj_x, self.traj_y, self.traj_yaw, self.speed_profile)

        # time = time + dt
        # check goal
        dx = self.state.x - goal[0]
        dy = self.state.y - goal[1]
        if math.hypot(dx, dy) <= goal_dis:
            print("Goal")

        self.x.append(self.state.x)
        self.y.append(self.state.y)
        self.yaw.append(self.state.yaw)
        self.v.append(self.state.v)
        # self.t.append(time)

        if target_ind % 1 == 0 and show_animation:
            plt.cla()
            # for stopping simulation with the esc key.
            plt.gcf().canvas.mpl_connect(
                'key_release_event',
                lambda event: [exit(0) if event.key == 'escape' else None])
            plt.plot(cx, cy, "-r", label="course")
            plt.plot(self.x, self.y, "ob", label="trajectory")
            plt.plot(cx[target_ind], cy[target_ind], "xg", label="target")
            plt.axis("equal")
            plt.grid(True)
            plt.title("speed[km/h]:" + str(round(self.state.v * 3.6, 2))
                    + ",target index:" + str(target_ind))
            plt.pause(0.0001)

        return self.t, self.x, self.y, self.yaw, self.v


def calc_speed_profile(cyaw, target_speed):
    speed_profile = [target_speed] * len(cyaw)

    direction = 1.0

    # Set stop point
    for i in range(len(cyaw) - 1):
        dyaw = abs(cyaw[i + 1] - cyaw[i])
        switch = math.pi / 4.0 <= dyaw < math.pi / 2.0

        if switch:
            direction *= -1

        if direction != 1.0:
            speed_profile[i] = - target_speed
        else:
            speed_profile[i] = target_speed

        if switch:
            speed_profile[i] = 0.0

    # speed down
    for i in range(40):
        speed_profile[-i] = target_speed / (50 - i)
        if speed_profile[-i] <= 1.0 / 3.6:
            speed_profile[-i] = 1.0 / 3.6

    return speed_profile


def main():

    # start ROS2 node
    rclpy.init()

    ax = [-5.0,0.0, 30.0, 60.0,90.0, 120.0]
    ay = [-0.0,0.0, 5.0, 15.0, 20.0, 25.0]
    goal = [ax[-1], ay[-1]]

    cx, cy, cyaw, ck, s = cubic_spline_planner.calc_spline_course(
        ax, ay, ds=0.1)
    target_speed = 10.0 / 3.6  # simulation parameter km/h -> m/s

    sp = calc_speed_profile(cyaw, target_speed)

    ros_control_sim = ROS_Control_Sim(cx, cy, cyaw, sp)



    rclpy.spin(ros_control_sim)

    # if show_animation:  # pragma: no cover
    #     plt.close()
    #     plt.subplots(1)
    #     plt.plot(ax, ay, "xb", label="waypoints")
    #     plt.plot(cx, cy, "-r", label="target course")
    #     plt.plot(x, y, "-g", label="tracking")
    #     plt.grid(True)
    #     plt.axis("equal")
    #     plt.xlabel("x[m]")
    #     plt.ylabel("y[m]")
    #     plt.legend()
    #     plt.subplots(1)

    #     plt.plot(t, np.array(v)*3.6, label="speed")
    #     plt.grid(True)
    #     plt.xlabel("Time [sec]")
    #     plt.ylabel("Speed [m/s]")
    #     plt.legend()

    #     plt.subplots(1)
    #     plt.plot(s, [np.rad2deg(iyaw) for iyaw in cyaw], "-r", label="yaw")
    #     plt.grid(True)
    #     plt.legend()
    #     plt.xlabel("line length[m]")
    #     plt.ylabel("yaw angle[deg]")

    #     plt.subplots(1)
    #     plt.plot(s, ck, "-r", label="curvature")
    #     plt.grid(True)
    #     plt.legend()
    #     plt.xlabel("line length[m]")
    #     plt.ylabel("curvature [1/m]")

    #     plt.show()


if __name__ == '__main__':
    main()