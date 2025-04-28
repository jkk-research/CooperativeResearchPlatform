/* header guard */
#ifndef _CAMERA_MODEL_HPP_
#define _CAMERA_MODEL_HPP_

// put includes here for the cpp file

#include <eigen3/Eigen/Dense>

#include <pcl/pcl_base.h>
#include <pcl/point_types.h>

#include "geometry_msgs/msg/point.hpp"
#include "sensor_msgs/msg/camera_info.hpp"
#include <utility> // for std::pair

// put function signatures here

/// @brief Pinhole Camera Model Class
/// @details This class is defines a pinhole camera model and 
/// provides basic functionalities like projecting 3D points 
/// to 2D points (projection) and vice versa (deprojection).
/// The camera model is defined by the intrinsic matrix.
class CameraModel {
public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW

    CameraModel() {}

    void setMatIntrinsic(const sensor_msgs::msg::CameraInfo & camera_info);

    /**
    * Projects a 3D point to a 2D point in the image plane.
    * 
    * @param point A 3D point in the camera's coordinate frame as pcl::PointXYZ or geometry_msgs::Point.
    * @param outPoint A reference to a std::pair<int, int> object that stores the projected coordinates (u, v).
    * 
    * Example usage for geometry_msgs::Point:
    * geometry_msgs::Point pointROS;
    * std::pair<int, int> pixel;
    * cameraModel.projectPoint(pointROS, pixel);
    * 
    * Example usage for pcl::PointXYZ:
    * pcl::PointXYZ pointPCL;
    * std::pair<int, int> pixel;
    * cameraModel.projectPoint(pointPCL, pixel);
    * 
    * Note: The pixel coordinates are returned as an std::pair object.
    * For example, if you have pixel coordinates `u` and `v`, you can access them as follows:
    * auto pixel = cameraModel.projectPoint(point);
    * int u = pixel.first;
    * int v = pixel.second;   
    */
    void projectPoint(const pcl::PointXYZ& point, std::pair<int, int>& outPoint);
    void projectPoint(const geometry_msgs::msg::Point& point, std::pair<int, int>& outPoint);
    
    /**
     * Deprojects pixel coordinates to a 3D point in the camera's coordinate frame.
     * 
     * @param pixel A std::pair<int, int> representing the pixel coordinates (u, v).
     * @param outPoint A reference to a 3D point object that stores the deprojected coordinates
     *                 as pcl::PointXYZ or geometry_msgs::Point.
     * @param flip_y_axis If true, flips the Y-axis of the resulting 3D point (needed in grid map).
     * 
     * Example usage for geometry_msgs::Point:
     * geometry_msgs::Point pointROS;
     * cameraModel.deprojectPoint(std::make_pair(u, v), pointROS, false);
     * 
     * Example usage for pcl::PointXYZ:
     * pcl::PointXYZ pointPCL;
     * cameraModel.deprojectPoint(std::make_pair(u, v), pointPCL, false);
     * 
     * Note: The pixel coordinates must be provided as an std::pair object.
     * For example, if you have pixel coordinates `u` and `v`, you can create the pair as follows:
     * auto pixel = std::make_pair(u, v);
     */
    void deprojectPoint(const std::pair<int, int>& point, geometry_msgs::msg::Point& outPoint, bool flip_y_axis = false);
    void deprojectPoint(const std::pair<int, int>& point, pcl::PointXYZ& outPoint, bool flip_y_axis = false);

private:
    Eigen::Matrix3d mat_intrinsic;
    Eigen::Matrix3d mat_intrinsic_inv;

    // normal vector of the ground plane (XY plane, flat ground assumption)
    Eigen::Vector3d norm_vec{ 0.0, 1.0, 0.0 };
};

#endif /* _CAMERA_MODEL_HPP_ */