#include "camera_model/camera_model.hpp"


void CameraModel::setMatIntrinsic(const sensor_msgs::msg::CameraInfo & camera_info) {
    mat_intrinsic << camera_info.k[0], camera_info.k[1], camera_info.k[2],
                     camera_info.k[3], camera_info.k[4], camera_info.k[5],
                     camera_info.k[6], camera_info.k[7], camera_info.k[8];
    
    // std::cerr << "Setting the following camera intrinsic matrix:\n" << mat_intrinsic << std::endl;
    // std::cerr.flush();

    mat_intrinsic_inv = mat_intrinsic.inverse(); 
}


void CameraModel::projectPoint(const pcl::PointXYZ& point, std::pair<int, int>& outPoint) {
    int u = int((mat_intrinsic(0,0) * point.x) / point.z + mat_intrinsic(0,2));
    int v = int((mat_intrinsic(1,1) * point.y) / point.z + mat_intrinsic(1,2));
    
    outPoint = std::make_pair(u, v);
}

void CameraModel::projectPoint(const geometry_msgs::msg::Point& point, std::pair<int, int>& outPoint) {
    int u = int((mat_intrinsic(0,0) * point.x) / point.z + mat_intrinsic(0,2));
    int v = int((mat_intrinsic(1,1) * point.y) / point.z + mat_intrinsic(1,2));
    
    outPoint = std::make_pair(u, v);
}


void CameraModel::deprojectPoint(const std::pair<int, int>& point, pcl::PointXYZ& outPoint, bool flip_y_axis) {
    Eigen::Vector3d point_h(point.first, point.second, 1.0);
    Eigen::Vector3d point_c = mat_intrinsic_inv * point_h;
    double denominator = norm_vec.dot(point_c);
    Eigen::Vector3d camera_coords = point_c / denominator;

    outPoint.x = camera_coords[2];
    outPoint.y = flip_y_axis ? camera_coords[0] * (-1) : camera_coords[0];
    outPoint.z = camera_coords[1];
}

void CameraModel::deprojectPoint(const std::pair<int, int>& point, geometry_msgs::msg::Point& outPoint, bool flip_y_axis) {
    Eigen::Vector3d point_h(point.first, point.second, 1.0);
    Eigen::Vector3d point_c = mat_intrinsic_inv * point_h;
    double denominator = norm_vec.dot(point_c);
    Eigen::Vector3d camera_coords = point_c / denominator;

    outPoint.x = camera_coords[2];
    outPoint.y = flip_y_axis ? camera_coords[0] * (-1) : camera_coords[0];
    outPoint.z = camera_coords[1];
}
