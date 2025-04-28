#include <future>
#include <cmath>

// ROS2 core
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "sensor_msgs/msg/image.hpp"
#include "sensor_msgs/msg/camera_info.hpp"
#include "nav_msgs/msg/occupancy_grid.hpp"
#include "geometry_msgs/msg/point.hpp"

// TF2 headers
#include "tf2_ros/buffer.h"
#include "tf2_ros/transform_listener.h"
#include "tf2_geometry_msgs/tf2_geometry_msgs.hpp"

// OpenCV and cv_bridge
#include <cv_bridge/cv_bridge.hpp>
#include <opencv2/opencv.hpp>

// custom library for camera model (and deprojection)
#include "camera_model/camera_model.hpp"

/**
* @brief GridMap struct that holds map parameters and offers helper methods
*/
struct GridMap {
    float position_x;
    float position_y;
    float cell_size;
    float length_x;
    float length_y;
    int grid_width;
    float topleft_x;
    float topleft_y;
    float bottomright_x;
    float bottomright_y;
    int cell_num_x;
    int cell_num_y;

    void initGrid(nav_msgs::msg::OccupancyGrid &grid) const {
        grid.header.frame_id = "ground_link";
        grid.info.resolution = cell_size;
        grid.info.width = static_cast<uint32_t>(length_x / cell_size);
        grid.info.height = static_cast<uint32_t>(length_y / cell_size);
        grid.info.origin.position.x = position_x - length_x / 2.0;  // switched to - sign
        grid.info.origin.position.y = position_y - length_y / 2.0;  // switched to - sign

        grid.info.origin.position.z = 0.0;
        grid.info.origin.orientation.w = 1.0;
        grid.info.origin.orientation.x = 0.0;
        grid.info.origin.orientation.y = 0.0;
        grid.info.origin.orientation.z = 0.0;
    }

    void paramRefresh() {
        topleft_x = position_x + length_x / 2.0;
        topleft_y = position_y + length_y / 2.0;
        bottomright_x = position_x - length_x / 2.0;
        bottomright_y = position_y - length_y / 2.0;
        cell_num_x = static_cast<int>(length_x / cell_size);
        cell_num_y = static_cast<int>(length_y / cell_size);
        grid_width = cell_num_x;
    }
};

class PrcpGridCamNode : public rclcpp::Node {
public:
    PrcpGridCamNode() : Node("prcp_grid_cam_pub") {

        // parameter declaration
        this->declare_parameter<std::string>("camera_info_topic", "camera_info_topic");
        this->declare_parameter<std::string>("camera_stream_topic", "camera_stream_topic");
        this->declare_parameter<float>("position_x", 0.0);
        this->declare_parameter<float>("position_y", 0.0);
        this->declare_parameter<float>("cell_size", 0.05);
        this->declare_parameter<float>("length_x", 10.0);
        this->declare_parameter<float>("length_y", 10.0);
        this->declare_parameter<float>("camera_horizon_offset", 0.0);
        this->declare_parameter<float>("pitch_in_deg", 0.0);

        // retrieve parameters
        camera_info_topic_   = this->get_parameter("camera_info_topic").as_string();
        camera_stream_topic_ = this->get_parameter("camera_stream_topic").as_string();
        
        // retrieve grid map parameters
        grid_map_.position_x = this->get_parameter("position_x").as_double();
        grid_map_.position_y = this->get_parameter("position_y").as_double();
        grid_map_.cell_size = this->get_parameter("cell_size").as_double();
        grid_map_.length_x = this->get_parameter("length_x").as_double();
        grid_map_.length_y = this->get_parameter("length_y").as_double();
        grid_map_.paramRefresh();

        // TF2 buffer and listener (for transform lookups)
        tf_buffer_ = std::make_shared<tf2_ros::Buffer>(this->get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);

        // callback to handle parameter changes at runtime (dynamic reconfigure alternative)
        auto param_callback_handle = this->add_on_set_parameters_callback(
            std::bind(
                &PrcpGridCamNode::onParameterChange,
                this,
                std::placeholders::_1)
            );
        (void)param_callback_handle; 

        // subscribers and publishers
        segmentation_sub_ = this->create_subscription<sensor_msgs::msg::Image>(
            camera_stream_topic_,
            1,
            std::bind(
                &PrcpGridCamNode::segToGridMapCallback,
                this,
                std::placeholders::_1)
        );

        occupancy_grid_pub_ = this->create_publisher<nav_msgs::msg::OccupancyGrid>(
            "/cam/grid",
            1
        );

        // read camera info and initialize the camera model
        initializeCameraModel();
  }

private:
    /* This callback updates the parameters dynamically. */
    rcl_interfaces::msg::SetParametersResult onParameterChange(const std::vector<rclcpp::Parameter> &parameters) {
        for (const auto & param : parameters) {
        if (param.get_name() == "cell_size") { grid_map_.cell_size = param.as_double(); }
        else if (param.get_name() == "position_x") { grid_map_.position_x = param.as_double(); }
        else if (param.get_name() == "position_y") { grid_map_.position_y = param.as_double(); }
        else if (param.get_name() == "length_x") { grid_map_.length_x = param.as_double(); }
        else if (param.get_name() == "length_y") { grid_map_.length_y = param.as_double(); }
        else if (param.get_name() == "camera_horizon_offset") { horizon_offset_ = param.as_double(); }
        else if (param.get_name() == "pitch_in_deg") { pitch_in_deg_ = param.as_double(); }
        }
        grid_map_.paramRefresh();
        rcl_interfaces::msg::SetParametersResult result;
        result.successful = true;
        result.reason = "Grid parameters updated!";
        return result;
    }    

    // member variables
    
    // GridMap struct instance (holding map parameters)
    GridMap grid_map_;
    
    // camera model instance (for deprojection)
    CameraModel camera_model;

    // occupancy grid message instance
    nav_msgs::msg::OccupancyGrid occupancy_grid_msg_;
    
    // road points vector (from segmentation)
    std::vector<geometry_msgs::msg::Point> road_points_;

    // runtime parameters
    double horizon_offset_{0.0};
    double pitch_in_deg_{0.0};
    
    // other parameters
    double resolution_{0.05};

    // parameters from public init
    std::string camera_info_topic_;
    std::string camera_stream_topic_;

    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;

    rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr segmentation_sub_;
    rclcpp::Publisher<nav_msgs::msg::OccupancyGrid>::SharedPtr occupancy_grid_pub_;


    // callback methods

    /**
     * @brief Callback to process segmentation image and create occupancy grid
     */
    void segToGridMapCallback(const sensor_msgs::msg::Image::SharedPtr msg)
    {
        if (msg->data.empty()) {
            RCLCPP_WARN(this->get_logger(), "Received empty message!");
            return;
        }

        cv_bridge::CvImagePtr cv_ptr;
        try {
            cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
        } 
        catch (const cv_bridge::Exception &e) {
            RCLCPP_ERROR(this->get_logger(), "cv_bridge exception: %s", e.what());
            return;
        }
        road_points_.clear();

        cv::Vec3b road_label_color(0, 255, 0);  // green color for road

        int rows = cv_ptr->image.rows;
        int cols = cv_ptr->image.cols;   

        // calculate the horizon line based on the camera_horizon_offset parameter.
        int horizon = rows / 2 + static_cast<int>(rows * horizon_offset_);

        // loop through pixels below the horizon
        for (int v = horizon; v < rows; ++v) {
            for (int u = 0; u < cols; ++u) {
                cv::Vec3b pixel = cv_ptr->image.at<cv::Vec3b>(v, u);
                if (pixel == road_label_color) {
                    // deproject pixel to a 3D point
                    geometry_msgs::msg::Point pt;
                    std::pair<int, int> pixel_pair(u, v);
                    camera_model.deprojectPoint(pixel_pair, pt, true);
                    road_points_.push_back(pt);

                    // if the pixel immediately below is also road, interpolate between them
                    if (v + 1 < rows) {
                        cv::Vec3b next_pixel = cv_ptr->image.at<cv::Vec3b>(v + 1, u);
                        if (next_pixel == road_label_color) {
                            geometry_msgs::msg::Point pt_next;
                            std::pair<int, int> next_pixel_pair(u, v + 1);
                            camera_model.deprojectPoint(next_pixel_pair, pt_next, true);
                            completeGridMap(pt, pt_next);
                        }
                    }
                }
            }
        }

        // after processing, create and publish the occupancy grid
        createGridMap(msg->header.frame_id);
    }


    // helper methods

    /**
    * @brief Compute index for a given (x,y) coordinate in the grid
    */
    int getIndex(double x, double y) {
        // convert the world coordinates to grid indices
        int cell_x = static_cast<int>((x - occupancy_grid_msg_.info.origin.position.x) / grid_map_.cell_size);
        int cell_y = static_cast<int>((y - occupancy_grid_msg_.info.origin.position.y) / grid_map_.cell_size);
        
        // check bounds; if out-of-bound, log a warning and return -1 so the caller can ignore this point
        if (cell_x < 0 || cell_x >= grid_map_.cell_num_x ||
            cell_y < 0 || cell_y >= grid_map_.cell_num_y) {
            // RCLCPP_WARN(
            //     this->get_logger(), 
            //     "Point (%.2f, %.2f) computed to grid cell (%d, %d) is out of bounds.",
            //     x, y, cell_x, cell_y
            // );
            return -1;
        }
        
        return cell_y * grid_map_.cell_num_x + cell_x;
    }

    /**
    * @brief Create and publish the occupancy grid based on road_points_
    */
    void createGridMap(const std::string &frame_id)
    {
        // init the occupancy grid message using grid_map_ settings
        grid_map_.initGrid(occupancy_grid_msg_);
        occupancy_grid_msg_.header.stamp = this->now();
        occupancy_grid_msg_.header.frame_id = "ground_link";
    
        // create a 1D occupancy vector (default value 100 = occupied)
        std::vector<int8_t> occ_points(grid_map_.cell_num_x * grid_map_.cell_num_y, 100);
    
        // lookup transform from input frame to "ground_link"
        geometry_msgs::msg::TransformStamped transform;
        try {
            transform = tf_buffer_->lookupTransform("ground_link", frame_id, tf2::TimePointZero);
        } catch (const tf2::TransformException &ex) {
            RCLCPP_ERROR(this->get_logger(), "TF lookup failed: %s", ex.what());
            return;
        }
    
        // for each road point, transform it and mark the corresponding grid cell as free (0)
        for (const auto & point : road_points_) {
            geometry_msgs::msg::Point point_out;
            tf2::doTransform(point, point_out, transform);
            int idx = getIndex(point_out.x, point_out.y);
            if (idx >= 0 && idx < static_cast<int>(occ_points.size())) {
            occ_points[idx] = 0;
            }
        }
        occupancy_grid_msg_.data = occ_points;
        occupancy_grid_pub_->publish(occupancy_grid_msg_);
    }


    void completeGridMap(const geometry_msgs::msg::Point &from, const geometry_msgs::msg::Point &to) {
        // calculate the direction from the origin toward the 'from' point
        double direction = std::atan2(from.y, from.x);
        double direction_cos = std::cos(direction);
        double direction_sin = std::sin(direction);

        // compute the starting and ending distances from the origin
        double start_range = std::sqrt(from.x * from.x + from.y * from.y);
        double end_range   = std::sqrt(to.x * to.x + to.y * to.y);

        // loop from start_range down to end_range, decrementing by the resolution
        for (double range = start_range; range >= end_range; range -= resolution_) {
            geometry_msgs::msg::Point p;
            p.x = range * direction_cos;
            p.y = range * direction_sin;
            p.z = 0.0;  // Set z accordingly if needed
            road_points_.push_back(p);
        }
    }


    /**
     * @brief Read the the camera info topic, then destroy the temporary 
     * subscription when it goes out of scope.
     */
    void initializeCameraModel() {
        // create a promise/future pair
        std::promise<sensor_msgs::msg::CameraInfo::SharedPtr> promise;
        auto future = promise.get_future();
    
        auto temp_subscription = this->create_subscription<sensor_msgs::msg::CameraInfo>(
            camera_info_topic_, 1,
            [&promise](const sensor_msgs::msg::CameraInfo::SharedPtr msg) {
                promise.set_value(msg);
            }
        );
        // spin until message is received
        rclcpp::spin_until_future_complete(this->get_node_base_interface(), future);
        auto camera_info_msg = future.get();
    
        // configure the camera model with the intrinsic matrix
        camera_model.setMatIntrinsic(*camera_info_msg);
        RCLCPP_INFO(this->get_logger(), "Camera info received; intrinsic matrix set.");
    }
};  


int main(int argc, char *argv[]) {
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PrcpGridCamNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}
