import rclpy
from rclpy.node import Node
from geometry_msgs.msg import PoseStamped
from std_msgs.msg import Float64
from autoware_auto_planning_msgs.msg import Trajectory,TrajectoryPoint
from autoware_auto_control_msgs.msg import AckermannControlCommand

class PIDController:
    def __init__(self, kp, ki, kd):
        self.kp = kp
        self.ki = ki
        self.kd = kd
        self.prev_error = 0
        self.integral = 0

    def compute(self, error):
        self.integral += error
        derivative = error - self.prev_error
        output = self.kp * error + self.ki * self.integral + self.kd * derivative
        self.prev_error = error
        return output

class CarControlNode(Node):
    def __init__(self):
        super().__init__('car_control_node')
        self.publishers = self.create_publisher(AckermannControlCommand, '/control/command/ctrl_cmd', 10)
        self.traj_publisher = self.create_subscription(Trajectory, '/planning/scenario_planning/trajectory', self.trajectory_callback,10)
        self.pid_controller = PIDController(0.25, 0.0, 0.0)  # Example PID values
        self.lateral_error = 0

    def trajectory_callback(self, msg):
        
        self.lateral_error = sum(msg.points[0:10].pose.position.y) / 10

    def control_loop(self):    
        # Example control logic
        setpoint = 0.0  # Example setpoint
        pv = self.current_pose.position.x  # Example process variable
        control_signal = self.pid_controller.compute(self.lateral_error)
        control_msg = Float64()
        control_msg.data = control_signal
        self.publisher.publish(control_msg)

def main(args=None):
    rclpy.init(args=args)
    node = CarControlNode()
    rate = node.create_rate(10)  # 10 Hz

    try:
        while rclpy.ok():
            rclpy.spin_once(node)
            node.control_loop()
            rate.sleep()
    except KeyboardInterrupt:
        pass

    node.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()