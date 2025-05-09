/**
 * @file patchworkpp.hpp
 * @author Seungjae Lee
 * @brief
 * @version 0.1
 * @date 2022-07-20
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef PATCHWORKPP_ROS2_H
#define PATCHWORKPP_ROS2_H

#include <sensor_msgs/msg/point_cloud2.hpp>
#include <rclcpp/rclcpp.hpp>
#include <eigen3/Eigen/Dense>
#include <boost/format.hpp>
#include <numeric>
#include <queue>
#include <mutex>
#include <patchworkpp/utils.hpp>
#include "rclcpp_components/register_node_macro.hpp"
#include "rcl_interfaces/msg/set_parameters_result.hpp"

#define MARKER_Z_VALUE -2.2
#define UPRIGHT_ENOUGH 0.55
#define FLAT_ENOUGH 0.2
#define TOO_HIGH_ELEVATION 0.0
#define TOO_TILTED 1.0

#define NUM_HEURISTIC_MAX_PTS_IN_PATCH 3000

using Eigen::MatrixXf;
using Eigen::JacobiSVD;
using Eigen::VectorXf;

using namespace std;

/*
    @brief PathWork ROS Node.
*/
template <typename PointT>
bool point_z_cmp(PointT a, PointT b) { return a.z < b.z; }

template <typename PointT>
struct RevertCandidate
{
    int concentric_idx;
    int sector_idx;
    double ground_flatness;
    double line_variable;
    Eigen::Vector4f pc_mean;
    pcl::PointCloud<PointT> regionwise_ground;

    RevertCandidate(int _c_idx, int _s_idx, double _flatness, double _line_var, Eigen::Vector4f _pc_mean, pcl::PointCloud<PointT> _ground)
     : concentric_idx(_c_idx), sector_idx(_s_idx), ground_flatness(_flatness), line_variable(_line_var), pc_mean(_pc_mean), regionwise_ground(_ground) {}
};

class PatchWorkpp : public rclcpp::Node {
public:
    typedef std::vector<pcl::PointCloud<PointXYZILID>> Ring;
    typedef std::vector<Ring> Zone;

    PatchWorkpp(const rclcpp::NodeOptions &options)
        : Node("patchworkpp", options) {
        // Init ROS related

        this->declare_parameter<double>("sensor_height", sensor_height_);
        this->declare_parameter<int>("num_iter", num_iter_);
        this->declare_parameter<int>("num_lpr", num_lpr_);
        this->declare_parameter<int>("num_min_pts", num_min_pts_);
        this->declare_parameter<double>("th_seeds", th_seeds_);
        this->declare_parameter<double>("th_dist", th_dist_);
        this->declare_parameter<double>("max_range", max_range_);
        this->declare_parameter<double>("min_range", min_range_);
        this->declare_parameter<double>("uprightness_thr", uprightness_thr_);
        this->declare_parameter<double>("adaptive_seed_selection_margin", adaptive_seed_selection_margin_);
        this->declare_parameter<double>("RNR_ver_angle_thr", RNR_ver_angle_thr_);

        this->declare_parameter<int>("num_zones", num_zones_);
        this->declare_parameter<double>("th_seeds_v", th_seeds_v_);
        this->declare_parameter<double>("th_dist_v", th_dist_v_);
        this->declare_parameter<double>("RNR_intensity_thr", RNR_intensity_thr_);
        this->declare_parameter<std::string>("cloud_topic", cloud_topic);
        this->declare_parameter<std::string>("frame_id", frame_id_);
        this->declare_parameter<bool>("verbose", verbose_);
        this->declare_parameter<bool>("display_time", display_time_);

        this->get_parameter<double>("sensor_height", sensor_height_);
        this->get_parameter<int>("num_iter", num_iter_);
        this->get_parameter<int>("num_lpr", num_lpr_);
        this->get_parameter<int>("num_min_pts", num_min_pts_);
        this->get_parameter<double>("th_seeds", th_seeds_);
        this->get_parameter<double>("th_dist", th_dist_);
        this->get_parameter<double>("max_range", max_range_);
        this->get_parameter<double>("min_range", min_range_);
        this->get_parameter<double>("uprightness_thr", uprightness_thr_);
        this->get_parameter<double>("adaptive_seed_selection_margin", adaptive_seed_selection_margin_);
        this->get_parameter<double>("RNR_ver_angle_thr", RNR_ver_angle_thr_);

        this->get_parameter<int>("num_zones", num_zones_);
        this->get_parameter<double>("th_seeds_v", th_seeds_v_);
        this->get_parameter<double>("th_dist_v", th_dist_v_);
        this->get_parameter<double>("RNR_intensity_thr", RNR_intensity_thr_);
        this->get_parameter<std::string>("cloud_topic", cloud_topic);
        this->get_parameter<std::string>("frame_id", frame_id_);
        this->get_parameter<bool>("verbose", verbose_);
        this->get_parameter<bool>("display_time", display_time_);

        RCLCPP_INFO_STREAM(this->get_logger(), "Inititalizing PatchWork++...");
        RCLCPP_INFO_STREAM(this->get_logger(), "Sensor Height: " << sensor_height_);
        RCLCPP_INFO_STREAM(this->get_logger(), "Num of Iteration: " <<  num_iter_);
        RCLCPP_INFO_STREAM(this->get_logger(), "Num of LPR: " <<  num_lpr_);
        RCLCPP_INFO_STREAM(this->get_logger(), "Num of min. points: " << num_min_pts_);
        RCLCPP_INFO_STREAM(this->get_logger(), "Seeds Threshold: "<< th_seeds_);
        RCLCPP_INFO_STREAM(this->get_logger(), "Distance Threshold: "<< th_dist_);
        RCLCPP_INFO_STREAM(this->get_logger(), "Max. range:: " << max_range_);
        RCLCPP_INFO_STREAM(this->get_logger(), "Min. range:: " << min_range_);
        RCLCPP_INFO_STREAM(this->get_logger(), "Normal vector threshold: " <<  uprightness_thr_);
        RCLCPP_INFO_STREAM(this->get_logger(), "adaptive_seed_selection_margin: " << adaptive_seed_selection_margin_);
        RCLCPP_INFO_STREAM(this->get_logger(), "RNR_ver_angle_thr: " << RNR_ver_angle_thr_);
        RCLCPP_INFO_STREAM(this->get_logger(), "Num. zones: " << num_zones_);
        RCLCPP_INFO_STREAM(this->get_logger(), "cloud_topic: " << cloud_topic);
        RCLCPP_INFO_STREAM(this->get_logger(), "frame_id: " << frame_id_);

        // CZM denotes 'Concentric Zone Model'. Please refer to our paper
        num_sectors_each_zone_ = std::vector<long>{8, 8, 8, 8};
        new_num_sectors_each_zone_   = std::vector<long>{8, 8, 8, 8}; 
        num_rings_each_zone_   = std::vector<long>{8, 8, 8, 8};
        new_num_rings_each_zone_   = std::vector<long>{8, 8, 8, 8};
        elevation_thr_ = std::vector<double>{0.0, 0.0, 0.0, 0.0};
        new_elevation_thr_ = std::vector<double>{0.0, 0.0, 0.0, 0.0};
        flatness_thr_  = std::vector<double>{0.0, 0.0, 0.0, 0.0};
        new_flatness_thr_  = std::vector<double>{0.0, 0.0, 0.0, 0.0};

        RCLCPP_INFO(rclcpp::get_logger("patchworkpp"), "Num. zones: %d", num_zones_);

        if (num_zones_ != 4 || num_sectors_each_zone_.size() != num_rings_each_zone_.size()) {
            throw invalid_argument("Some parameters are wrong! Check the num_zones and num_rings/sectors_each_zone");
        }
        if (elevation_thr_.size() != flatness_thr_.size()) {
            throw invalid_argument("Some parameters are wrong! Check the elevation/flatness_thresholds");
        }

        cout << (boost::format("Num. sectors: %d, %d, %d, %d") % num_sectors_each_zone_[0] % num_sectors_each_zone_[1] %
                 num_sectors_each_zone_[2] %
                 num_sectors_each_zone_[3]).str() << endl;
        cout << (boost::format("Num. rings: %01d, %01d, %01d, %01d") % num_rings_each_zone_[0] %
                 num_rings_each_zone_[1] %
                 num_rings_each_zone_[2] %
                 num_rings_each_zone_[3]).str() << endl;
        cout << (boost::format("elevation_thr_: %0.4f, %0.4f, %0.4f, %0.4f ") % elevation_thr_[0] % elevation_thr_[1] %
                 elevation_thr_[2] %
                 elevation_thr_[3]).str() << endl;
        cout << (boost::format("flatness_thr_: %0.4f, %0.4f, %0.4f, %0.4f ") % flatness_thr_[0] % flatness_thr_[1] %
                 flatness_thr_[2] %
                 flatness_thr_[3]).str() << endl;
        num_rings_of_interest_ = elevation_thr_.size();

        int num_polygons = std::inner_product(num_rings_each_zone_.begin(), num_rings_each_zone_.end(), num_sectors_each_zone_.begin(), 0);
        revert_pc_.reserve(NUM_HEURISTIC_MAX_PTS_IN_PATCH);
        ground_pc_.reserve(NUM_HEURISTIC_MAX_PTS_IN_PATCH);
        regionwise_ground_.reserve(NUM_HEURISTIC_MAX_PTS_IN_PATCH);
        regionwise_nonground_.reserve(NUM_HEURISTIC_MAX_PTS_IN_PATCH);

        pub_revert_pc = Node::create_publisher<sensor_msgs::msg::PointCloud2>("plane", 100);
        pub_reject_pc = Node::create_publisher<sensor_msgs::msg::PointCloud2>("plane", 100);
        pub_normal = Node::create_publisher<sensor_msgs::msg::PointCloud2>("plane", 100);
        pub_noise = Node::create_publisher<sensor_msgs::msg::PointCloud2>("plane", 100);
        pub_vertical = Node::create_publisher<sensor_msgs::msg::PointCloud2>("plane", 100);

        min_range_z2_ = (7 * min_range_ + max_range_) / 8.0;
        min_range_z3_ = (3 * min_range_ + max_range_) / 4.0;
        min_range_z4_ = (min_range_ + max_range_) / 2.0;
        min_ranges_ = {min_range_, min_range_z2_, min_range_z3_, min_range_z4_};
        ring_sizes_ = {(min_range_z2_ - min_range_) / num_rings_each_zone_.at(0),
                      (min_range_z3_ - min_range_z2_) / num_rings_each_zone_.at(1),
                      (min_range_z4_ - min_range_z3_) / num_rings_each_zone_.at(2),
                      (max_range_ - min_range_z4_) / num_rings_each_zone_.at(3)};
        sector_sizes_ = {2 * M_PI / num_sectors_each_zone_.at(0), 2 * M_PI / num_sectors_each_zone_.at(1),
                        2 * M_PI / num_sectors_each_zone_.at(2),
                        2 * M_PI / num_sectors_each_zone_.at(3)};

        cout << "INITIALIZATION COMPLETE" << endl;
        for (int i = 0; i < num_zones_; i++) {
            Zone z;
            initialize_zone(z, num_sectors_each_zone_[i], num_rings_each_zone_[i]);
            ConcentricZoneModel_.push_back(z);
        }

        pub_cloud = Node::create_publisher<sensor_msgs::msg::PointCloud2>("cloud", 100);
        pub_ground = Node::create_publisher<sensor_msgs::msg::PointCloud2>("ground", 100);
        pub_non_ground = Node::create_publisher<sensor_msgs::msg::PointCloud2>("nonground", 100);
        sub_cloud = Node::create_subscription<sensor_msgs::msg::PointCloud2>(cloud_topic, rclcpp::SensorDataQoS().keep_last(1), std::bind(&PatchWorkpp::callbackCloud, this, std::placeholders::_1));    
        callback_handle_ = this->add_on_set_parameters_callback(std::bind(&PatchWorkpp::parametersCallback, this, std::placeholders::_1));

    };

    void estimate_ground(pcl::PointCloud<PointXYZILID> cloud_in, pcl::PointCloud<PointXYZILID> &cloud_ground, pcl::PointCloud<PointXYZILID> &cloud_nonground, double &time_taken);

private:

    // Every private member variable is written with the undescore("_") in its end.
    std::recursive_mutex mutex_;

    int num_iter_ =  3;
    int num_lpr_ = 20;
    int num_min_pts_ = 15 ;
    int num_zones_ = 4;
    int num_rings_of_interest_;
    std::string cloud_topic = "/kitti/point_cloud";
    std::string frame_id_ = "test";
    double sensor_height_ =  1.0;
    double th_seeds_ = 0.3;
    double th_dist_ = 0.4;
    double th_seeds_v_ = 0.25;
    double th_dist_v_ = 0.1;
    double max_range_ = 80.0;
    double min_range_ = 0.0;
    double uprightness_thr_ =  0.707;
    double adaptive_seed_selection_margin_;
    double min_range_z2_ = 12.3625; // 12.3625
    double min_range_z3_ = 22.025; // 22.025
    double min_range_z4_ = 41.35; // 41.35
    double RNR_ver_angle_thr_ = -20.0;
    double RNR_intensity_thr_ = 0.2;
    bool verbose_  = false;
    bool display_time_ = false; // another verbose option, displays running_time
    bool enable_RNR_ = true;
    bool enable_RVPF_= true;
    bool enable_TGR_= true;

    int max_flatness_storage_, max_elevation_storage_;
    std::vector<double> update_flatness_[4];
    std::vector<double> update_elevation_[4];

    float d_;

    VectorXf normal_;
    MatrixXf pnormal_;
    VectorXf singular_values_;
    Eigen::Matrix3f cov_;
    Eigen::Vector4f pc_mean_;

    // For visualization
    bool visualize_ = true;
    vector<long> new_num_sectors_each_zone_;
    vector<long> new_num_rings_each_zone_;
    vector<double> new_flatness_thr_;
    vector<double> new_elevation_thr_;
    vector<long> num_sectors_each_zone_;
    vector<long> num_rings_each_zone_;
    vector<double> sector_sizes_;
    vector<double> ring_sizes_;
    vector<double> min_ranges_;
    vector<double> elevation_thr_;
    vector<double> flatness_thr_;
    queue<int> noise_idxs_;
    vector<Zone> ConcentricZoneModel_;

    // ros::Publisher PlaneViz, 
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_revert_pc, pub_reject_pc, pub_normal, pub_noise, pub_vertical, pub_cloud, pub_ground, pub_non_ground;
    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr sub_cloud;
    OnSetParametersCallbackHandle::SharedPtr callback_handle_;
    pcl::PointCloud<PointXYZILID> revert_pc_, reject_pc_, noise_pc_, vertical_pc_;
    pcl::PointCloud<PointXYZILID> ground_pc_;
    pcl::PointCloud<pcl::PointXYZINormal> normals_;
    pcl::PointCloud<PointXYZILID> regionwise_ground_, regionwise_nonground_;

    void initialize_zone(Zone &z, int num_sectors, int num_rings);
    void flush_patches_in_zone(Zone &patches, int num_sectors, int num_rings);
    void flush_patches(std::vector<Zone> &czm);
    void pc2czm(const pcl::PointCloud<PointXYZILID> &src, std::vector<Zone> &czm, pcl::PointCloud<PointXYZILID> &cloud_nonground);
    void reflected_noise_removal(pcl::PointCloud<PointXYZILID> &cloud, pcl::PointCloud<PointXYZILID> &cloud_nonground);
    void temporal_ground_revert(pcl::PointCloud<PointXYZILID> &cloud_ground, pcl::PointCloud<PointXYZILID> &cloud_nonground,
                                std::vector<double> ring_flatness, std::vector<RevertCandidate<PointXYZILID>> candidates,
                                int concentric_idx);

    void calc_mean_stdev(std::vector<double> vec, double &mean, double &stdev);
    void update_elevation_thr();
    void update_flatness_thr();
    double xy2theta(const double &x, const double &y);
    double xy2radius(const double &x, const double &y);
    void estimate_plane(const pcl::PointCloud<PointXYZILID> &ground);
    void extract_piecewiseground(
            const int zone_idx, const pcl::PointCloud<PointXYZILID> &src,
            pcl::PointCloud<PointXYZILID> &dst,
            pcl::PointCloud<PointXYZILID> &non_ground_dst);
    void extract_initial_seeds(
            const int zone_idx, const pcl::PointCloud<PointXYZILID> &p_sorted,
            pcl::PointCloud<PointXYZILID> &init_seeds);
    void extract_initial_seeds(
            const int zone_idx, const pcl::PointCloud<PointXYZILID> &p_sorted,
            pcl::PointCloud<PointXYZILID> &init_seeds, double th_seed);
    void callbackCloud(const sensor_msgs::msg::PointCloud2::ConstSharedPtr &cloud_msg);

    /* ROS Callbacks Functions */
    rcl_interfaces::msg::SetParametersResult parametersCallback(
        const std::vector<rclcpp::Parameter> &parameters);
    sensor_msgs::msg::PointCloud2 cloud2msg(pcl::PointCloud<PointXYZILID> cloud, const rclcpp::Time& stamp, std::string frame_id); 
};

inline
void PatchWorkpp::initialize_zone(Zone &z, int num_sectors, int num_rings) {
    z.clear();
    pcl::PointCloud<PointXYZILID> cloud;
    cloud.reserve(1000);
    Ring ring;
    for (int i = 0; i < num_sectors; i++) {
        ring.emplace_back(cloud);
    }
    for (int j = 0; j < num_rings; j++) {
        z.emplace_back(ring);
    }
}

inline
void PatchWorkpp::flush_patches_in_zone(Zone &patches, int num_sectors, int num_rings) {
    for (int i = 0; i < num_sectors; i++) {
        for (int j = 0; j < num_rings; j++) {
            if (!patches[j][i].points.empty()) patches[j][i].points.clear();
        }
    }
}

inline
void PatchWorkpp::flush_patches(vector<Zone> &czm) {
    for (int k = 0; k < num_zones_; k++) {
        for (int i = 0; i < num_rings_each_zone_[k]; i++) {
            for (int j = 0; j < num_sectors_each_zone_[k]; j++) {
                if (!czm[k][i][j].points.empty()) czm[k][i][j].points.clear();
            }
        }
    }

    if( verbose_ ) cout << "Flushed patches" << endl;
}

inline
void PatchWorkpp::estimate_plane(const pcl::PointCloud<PointXYZILID> &ground) {
    pcl::computeMeanAndCovarianceMatrix(ground, cov_, pc_mean_);
    // Singular Value Decomposition: SVD
    Eigen::JacobiSVD<Eigen::MatrixXf> svd(cov_, Eigen::DecompositionOptions::ComputeFullU);
    singular_values_ = svd.singularValues();

    // use the least singular vector as normal
    normal_ = (svd.matrixU().col(2));

    if (normal_(2) < 0) { for(int i=0; i<3; i++) normal_(i) *= -1; }

    // mean ground seeds value
    Eigen::Vector3f seeds_mean = pc_mean_.head<3>();

    // according to normal.T*[x,y,z] = -d
    d_ = -(normal_.transpose() * seeds_mean)(0, 0);
}

inline
void PatchWorkpp::extract_initial_seeds(
        const int zone_idx, const pcl::PointCloud<PointXYZILID> &p_sorted,
        pcl::PointCloud<PointXYZILID> &init_seeds, double th_seed) {
    init_seeds.points.clear();

    // LPR is the mean of low point representative
    double sum = 0;
    int cnt = 0;

    int init_idx = 0;
    if (zone_idx == 0) {
        for (int i = 0; i < p_sorted.points.size(); i++) {
            if (p_sorted.points[i].z < adaptive_seed_selection_margin_ * sensor_height_) {
                ++init_idx;
            } else {
                break;
            }
        }
    }

    // Calculate the mean height value.
    for (int i = init_idx; i < p_sorted.points.size() && cnt < num_lpr_; i++) {
        sum += p_sorted.points[i].z;
        cnt++;
    }
    double lpr_height = cnt != 0 ? sum / cnt : 0;// in case divide by 0

    // iterate pointcloud, filter those height is less than lpr.height+th_seeds_
    for (int i = 0; i < p_sorted.points.size(); i++) {
        if (p_sorted.points[i].z < lpr_height + th_seed) {
            init_seeds.points.push_back(p_sorted.points[i]);
        }
    }
}

inline
void PatchWorkpp::extract_initial_seeds(
        const int zone_idx, const pcl::PointCloud<PointXYZILID> &p_sorted,
        pcl::PointCloud<PointXYZILID> &init_seeds) {
    init_seeds.points.clear();

    // LPR is the mean of low point representative
    double sum = 0;
    int cnt = 0;
    int init_idx = 0;
    if (zone_idx == 0) {
        for (int i = 0; i < p_sorted.points.size(); i++) {
            if (p_sorted.points[i].z < adaptive_seed_selection_margin_ * sensor_height_) {
                ++init_idx;
            } else {
                break;
            }
        }
    }

    // Calculate the mean height value.
    for (int i = init_idx; i < p_sorted.points.size() && cnt < num_lpr_; i++) {
        sum += p_sorted.points[i].z;
        cnt++;
    }
    double lpr_height = cnt != 0 ? sum / cnt : 0;// in case divide by 0

    // iterate pointcloud, filter those height is less than lpr.height+th_seeds_
    for (int i = 0; i < p_sorted.points.size(); i++) {
        if (p_sorted.points[i].z < lpr_height + th_seeds_) {
            init_seeds.points.push_back(p_sorted.points[i]);
        }
    }
}

inline
void PatchWorkpp::reflected_noise_removal(pcl::PointCloud<PointXYZILID> &cloud_in, pcl::PointCloud<PointXYZILID> &cloud_nonground)
{
    for (int i=0; i<cloud_in.size(); i++)
    {
        double r = sqrt( cloud_in[i].x*cloud_in[i].x + cloud_in[i].y*cloud_in[i].y );
        double z = cloud_in[i].z;
        double ver_angle_in_deg = atan2(z, r)*180/M_PI;

        if ( ver_angle_in_deg < RNR_ver_angle_thr_ && z < -sensor_height_-0.8 && cloud_in[i].intensity < RNR_intensity_thr_)
        {
            cloud_nonground.push_back(cloud_in[i]);
            noise_pc_.push_back(cloud_in[i]);
            noise_idxs_.push(i);
        }
    }

    if (verbose_) cout << "[ RNR ] Num of noises : " << noise_pc_.points.size() << endl;
}

inline
rcl_interfaces::msg::SetParametersResult PatchWorkpp::parametersCallback(
    const std::vector<rclcpp::Parameter> &parameters)
{
    rcl_interfaces::msg::SetParametersResult result;
    result.successful = true;
    result.reason = "success";
    for (const auto &param : parameters)
    {
        RCLCPP_INFO_STREAM(this->get_logger(), "Param updated: " << param.get_name().c_str() << ": " << param.value_to_string().c_str());
        if(param.get_name() == "cloud_topic" or param.get_name() == "frame_id")
        {
            cloud_topic = param.value_to_string();
            sub_cloud = Node::create_subscription<sensor_msgs::msg::PointCloud2>(cloud_topic, rclcpp::SensorDataQoS().keep_last(1), std::bind(&PatchWorkpp::callbackCloud, this, std::placeholders::_1));    
        }
        if(param.get_name() == "sensor_height")
        {
            sensor_height_ = param.as_double();
        }
        if(param.get_name() == "num_iter")
        {
            num_iter_ = param.as_int();
        }
        if(param.get_name() == "num_lpr")
        {
            num_lpr_ = param.as_int();
        }
        if(param.get_name() == "num_min_pts")
        {
            num_min_pts_ = param.as_int();
        }
        if(param.get_name() == "th_seeds")
        {
            th_seeds_ = param.as_double();
        }
        if(param.get_name() == "th_dist")
        {
            th_dist_ = param.as_double();
        }
        if(param.get_name() == "max_range")
        {
            max_range_ = param.as_double();
        }
        if(param.get_name() == "min_range")
        {
            min_range_ = param.as_double();
        }
        if(param.get_name() == "uprightness_thr")
        {
            uprightness_thr_ = param.as_double();
        }
        if(param.get_name() == "adaptive_seed_selection_margin")
        {
            adaptive_seed_selection_margin_ = param.as_double();
        }
        if(param.get_name() == "RNR_ver_angle_thr")
        {
            RNR_ver_angle_thr_ = param.as_double();
        }
        if(param.get_name() == "RNR_intensity_thr")
        {
            RNR_intensity_thr_ = param.as_double();
        }
        if(param.get_name() == "num_zones")
        {
            num_zones_ = param.as_int();
        }
        if(param.get_name() == "th_seeds_v")
        {
            th_seeds_v_ = param.as_double();
        }
        if(param.get_name() == "th_dist_v")
        {
            th_dist_v_ = param.as_double();
        }
        if(param.get_name() == "verbose")
        {
            verbose_ = param.as_bool();
        }
        if(param.get_name() == "display_time")
        {
            display_time_ = param.as_bool();
        }

    }

    return result;
}

/*
    @brief Velodyne pointcloud callback function. The main GPF pipeline is here.
    PointCloud SensorMsg -> Pointcloud -> z-value sorted Pointcloud
    ->error points removal -> extract ground seeds -> ground plane fit mainloop
*/

inline
void PatchWorkpp::estimate_ground(
        pcl::PointCloud<PointXYZILID> cloud_in,
        pcl::PointCloud<PointXYZILID> &cloud_ground,
        pcl::PointCloud<PointXYZILID> &cloud_nonground,
        double &time_taken) {

    unique_lock<recursive_mutex> lock(mutex_);
    static double start, t0, t1, t2, end;

    double pca_time_ = 0.0;
    double t_revert = 0.0;
    double t_total_ground = 0.0;
    double t_total_estimate = 0.0;

    start = rclcpp::Clock{RCL_STEADY_TIME}.now().seconds();

    cloud_ground.clear();
    cloud_nonground.clear();

    // 1. Reflected Noise Removal (RNR)
    if (enable_RNR_) reflected_noise_removal(cloud_in, cloud_nonground);

    t1 = rclcpp::Clock{RCL_STEADY_TIME}.now().seconds();

    // 2. Concentric Zone Model (CZM)
    flush_patches(ConcentricZoneModel_);
    pc2czm(cloud_in, ConcentricZoneModel_, cloud_nonground);

    t2 = rclcpp::Clock{RCL_STEADY_TIME}.now().seconds();

    int concentric_idx = 0;

    double t_sort = 0;

    std::vector<RevertCandidate<PointXYZILID>> candidates;
    std::vector<double> ringwise_flatness;

    for (int zone_idx = 0; zone_idx < num_zones_; ++zone_idx) {

        auto zone = ConcentricZoneModel_[zone_idx];

        for (int ring_idx = 0; ring_idx < num_rings_each_zone_[zone_idx]; ++ring_idx) {
            for (int sector_idx = 0; sector_idx < num_sectors_each_zone_[zone_idx]; ++sector_idx) {

                if (zone[ring_idx][sector_idx].points.size() < num_min_pts_)
                {
                    cloud_nonground += zone[ring_idx][sector_idx];
                    continue;
                }

                // --------- region-wise sorting (faster than global sorting method) ---------------- //
                double t_sort_0 = rclcpp::Clock{RCL_STEADY_TIME}.now().seconds();

                sort(zone[ring_idx][sector_idx].points.begin(), zone[ring_idx][sector_idx].points.end(), point_z_cmp<PointXYZILID>);

                double t_sort_1 = rclcpp::Clock{RCL_STEADY_TIME}.now().seconds();
                t_sort += (t_sort_1 - t_sort_0);
                // ---------------------------------------------------------------------------------- //

                double t_tmp0 = rclcpp::Clock{RCL_STEADY_TIME}.now().seconds();
                extract_piecewiseground(zone_idx, zone[ring_idx][sector_idx], regionwise_ground_, regionwise_nonground_);

                double t_tmp1 = rclcpp::Clock{RCL_STEADY_TIME}.now().seconds();
                t_total_ground += t_tmp1 - t_tmp0;
                pca_time_ += t_tmp1 - t_tmp0;

                // Status of each patch
                // used in checking uprightness, elevation, and flatness, respectively
                const double ground_uprightness = normal_(2);
                const double ground_elevation   = pc_mean_(2, 0);
                const double ground_flatness    = singular_values_.minCoeff();
                const double line_variable      = singular_values_(1) != 0 ? singular_values_(0)/singular_values_(1) : std::numeric_limits<double>::max();

                double heading = 0.0;
                for(int i=0; i<3; i++) heading += pc_mean_(i,0)*normal_(i);


                double t_tmp2 = rclcpp::Clock{RCL_STEADY_TIME}.now().seconds();

                /*
                    About 'is_heading_outside' condidition, heading should be smaller than 0 theoretically.
                    ( Imagine the geometric relationship between the surface normal vector on the ground plane and
                        the vector connecting the sensor origin and the mean point of the ground plane )

                    However, when the patch is far awaw from the sensor origin,
                    heading could be larger than 0 even if it's ground due to lack of amount of ground plane points.

                    Therefore, we only check this value when concentric_idx < num_rings_of_interest ( near condition )
                */
                bool is_upright         = ground_uprightness > uprightness_thr_;
                bool is_not_elevated    = ground_elevation < elevation_thr_[concentric_idx];
                bool is_flat            = ground_flatness < flatness_thr_[concentric_idx];
                bool is_near_zone       = concentric_idx < num_rings_of_interest_;
                bool is_heading_outside = heading < 0.0;

                /*
                    Store the elevation & flatness variables
                    for A-GLE (Adaptive Ground Likelihood Estimation)
                    and TGR (Temporal Ground Revert). More information in the paper Patchwork++.
                */
                if (is_upright && is_not_elevated && is_near_zone)
                {
                    update_elevation_[concentric_idx].push_back(ground_elevation);
                    update_flatness_[concentric_idx].push_back(ground_flatness);

                    ringwise_flatness.push_back(ground_flatness);
                }

                // Ground estimation based on conditions
                if (!is_upright)
                {
                    cloud_nonground += regionwise_ground_;
                }
                else if (!is_near_zone)
                {
                    cloud_ground += regionwise_ground_;
                }
                else if (!is_heading_outside)
                {
                    cloud_nonground += regionwise_ground_;
                }
                else if (is_not_elevated || is_flat)
                {
                    for (const auto& point : regionwise_ground_.points) {
                        PointXYZILID new_point = point;
                        cloud_ground.push_back(new_point);
                    }
                }
                else
                {
                    RevertCandidate<PointXYZILID> candidate(concentric_idx, sector_idx, ground_flatness, line_variable, pc_mean_, regionwise_ground_);
                    candidates.push_back(candidate);
                }
                // Every regionwise_nonground is considered nonground.
                for (const auto& point : regionwise_nonground_.points) {
                    PointXYZILID new_point = point;
                    cloud_nonground.push_back(new_point);
                }

                double t_tmp3 = rclcpp::Clock{RCL_STEADY_TIME}.now().seconds();
                t_total_estimate += t_tmp3 - t_tmp2;
            }

            double t_bef_revert = rclcpp::Clock{RCL_STEADY_TIME}.now().seconds();

            if (!candidates.empty())
            {
                if (enable_TGR_)
                {
                    temporal_ground_revert(cloud_ground, cloud_nonground, ringwise_flatness, candidates, concentric_idx);
                }
                else
                {
                    for (size_t i=0; i<candidates.size(); i++)
                    {
                        cloud_nonground += candidates[i].regionwise_ground;
                    }
                }

                candidates.clear();
                ringwise_flatness.clear();
            }

            double t_aft_revert = rclcpp::Clock{RCL_STEADY_TIME}.now().seconds();

            t_revert += t_aft_revert - t_bef_revert;

            concentric_idx++;
        }
    }

    double t_update = rclcpp::Clock{RCL_STEADY_TIME}.now().seconds();

    update_elevation_thr();
    update_flatness_thr();

    end = rclcpp::Clock{RCL_STEADY_TIME}.now().seconds();
    time_taken = end - start;

    if (visualize_)
    {
        sensor_msgs::msg::PointCloud2 cloud_ROS;
        pcl::toROSMsg(revert_pc_, cloud_ROS);
        cloud_ROS.header.stamp = rclcpp::Clock{RCL_STEADY_TIME}.now();
        cloud_ROS.header.frame_id = cloud_in.header.frame_id;
        pub_revert_pc->publish(cloud_ROS);

        pcl::toROSMsg(reject_pc_, cloud_ROS);
        cloud_ROS.header.stamp = rclcpp::Clock{RCL_STEADY_TIME}.now();
        cloud_ROS.header.frame_id = cloud_in.header.frame_id;
        pub_reject_pc->publish(cloud_ROS);

        pcl::toROSMsg(normals_, cloud_ROS);
        cloud_ROS.header.stamp = rclcpp::Clock{RCL_STEADY_TIME}.now();
        cloud_ROS.header.frame_id = cloud_in.header.frame_id;
        pub_normal->publish(cloud_ROS);

        pcl::toROSMsg(noise_pc_, cloud_ROS);
        cloud_ROS.header.stamp = rclcpp::Clock{RCL_STEADY_TIME}.now();
        cloud_ROS.header.frame_id = cloud_in.header.frame_id;
        pub_noise->publish(cloud_ROS);

        pcl::toROSMsg(vertical_pc_, cloud_ROS);
        cloud_ROS.header.stamp = rclcpp::Clock{RCL_STEADY_TIME}.now();
        cloud_ROS.header.frame_id = cloud_in.header.frame_id;
        pub_vertical->publish(cloud_ROS);
    }

    revert_pc_.clear();
    reject_pc_.clear();
    normals_.clear();
    noise_pc_.clear();
    vertical_pc_.clear();
}

inline
void PatchWorkpp::update_elevation_thr(void)
{
    for (int i=0; i<num_rings_of_interest_; i++)
    {
        if (update_elevation_[i].empty()) continue;

        double update_mean = 0.0, update_stdev = 0.0;
        calc_mean_stdev(update_elevation_[i], update_mean, update_stdev);
        if (i==0) {
            elevation_thr_[i] = update_mean + 3*update_stdev;
            sensor_height_ = -update_mean;
        }
        else elevation_thr_[i] = update_mean + 2*update_stdev;

        int exceed_num = update_elevation_[i].size() - max_elevation_storage_;
        if (exceed_num > 0) update_elevation_[i].erase(update_elevation_[i].begin(), update_elevation_[i].begin() + exceed_num);
    }

    if (verbose_)
    {
        cout << "sensor height: " << sensor_height_ << endl;
        cout << (boost::format("elevation_thr_  :   %0.4f,  %0.4f,  %0.4f,  %0.4f")
                % elevation_thr_[0] % elevation_thr_[1] % elevation_thr_[2] % elevation_thr_[3]).str() << endl;
    }

    return;
}

inline
void PatchWorkpp::update_flatness_thr(void)
{
    for (int i=0; i<num_rings_of_interest_; i++)
    {
        if (update_flatness_[i].empty()) break;
        if (update_flatness_[i].size() <= 1) break;

        double update_mean = 0.0, update_stdev = 0.0;
        calc_mean_stdev(update_flatness_[i], update_mean, update_stdev);
        flatness_thr_[i] = update_mean+update_stdev;

        int exceed_num = update_flatness_[i].size() - max_flatness_storage_;
        if (exceed_num > 0) update_flatness_[i].erase(update_flatness_[i].begin(), update_flatness_[i].begin() + exceed_num);
    }

    if (verbose_)
    {
        cout << (boost::format("flatness_thr_   :   %0.4f,  %0.4f,  %0.4f,  %0.4f")
                % flatness_thr_[0] % flatness_thr_[1] % flatness_thr_[2] % flatness_thr_[3]).str() << endl;
    }

    return;
}

inline
void PatchWorkpp::temporal_ground_revert(pcl::PointCloud<PointXYZILID> &cloud_ground, pcl::PointCloud<PointXYZILID> &cloud_nonground,
                                               std::vector<double> ring_flatness, std::vector<RevertCandidate<PointXYZILID>> candidates,
                                               int concentric_idx)
{
    if (verbose_) std::cout << "\033[1;34m" << "=========== Temporal Ground Revert (TGR) ===========" << "\033[0m" << endl;

    double mean_flatness = 0.0, stdev_flatness = 0.0;
    calc_mean_stdev(ring_flatness, mean_flatness, stdev_flatness);

    if (verbose_)
    {
        cout << "[" << candidates[0].concentric_idx << ", " << candidates[0].sector_idx << "]"
             << " mean_flatness: " << mean_flatness << ", stdev_flatness: " << stdev_flatness << std::endl;
    }

    for( size_t i=0; i<candidates.size(); i++ )
    {
        RevertCandidate<PointXYZILID> candidate = candidates[i];

        // Debug
        if(verbose_)
        {
            cout << "\033[1;33m" << candidate.sector_idx << "th flat_sector_candidate"
                 << " / flatness: " << candidate.ground_flatness
                 << " / line_variable: " << candidate.line_variable
                 << " / ground_num : " << candidate.regionwise_ground.size()
                 << "\033[0m" << endl;
        }

        double mu_flatness = mean_flatness + 1.5*stdev_flatness;
        double prob_flatness = 1/(1+exp( (candidate.ground_flatness-mu_flatness)/(mu_flatness/10) ));

        if (candidate.regionwise_ground.size() > 1500 && candidate.ground_flatness < th_dist_*th_dist_) prob_flatness = 1.0;

        double prob_line = 1.0;
        if (candidate.line_variable > 8.0 )//&& candidate.line_dir > M_PI/4)// candidate.ground_elevation > elevation_thr_[concentric_idx])
        {
            // if (verbose_) cout << "line_dir: " << candidate.line_dir << endl;
            prob_line = 0.0;
        }

        bool revert = prob_line*prob_flatness > 0.5;

        if ( concentric_idx < num_rings_of_interest_ )
        {
            if (revert)
            {
                if (verbose_)
                {
                    cout << "\033[1;32m" << "REVERT TRUE" << "\033[0m" << endl;
                }

                revert_pc_ += candidate.regionwise_ground;
                cloud_ground += candidate.regionwise_ground;
            }
            else
            {
                if (verbose_)
                {
                    cout << "\033[1;31m" << "FINAL REJECT" << "\033[0m" << endl;
                }
                reject_pc_ += candidate.regionwise_ground;
                cloud_nonground += candidate.regionwise_ground;
            }
        }
    }

    if (verbose_) std::cout << "\033[1;34m" << "====================================================" << "\033[0m" << endl;
}

// For adaptive
inline
void PatchWorkpp::extract_piecewiseground(
        const int zone_idx, const pcl::PointCloud<PointXYZILID> &src,
        pcl::PointCloud<PointXYZILID> &dst,
        pcl::PointCloud<PointXYZILID> &non_ground_dst) {

    // 0. Initialization
    if (!ground_pc_.empty()) ground_pc_.clear();
    if (!dst.empty()) dst.clear();
    if (!non_ground_dst.empty()) non_ground_dst.clear();

    // 1. Region-wise Vertical Plane Fitting (R-VPF)
    // : removes potential vertical plane under the ground plane
    pcl::PointCloud<PointXYZILID> src_wo_verticals;
    src_wo_verticals = src;

    if (enable_RVPF_)
    {
        for (int i = 0; i < num_iter_; i++)
        {
            extract_initial_seeds(zone_idx, src_wo_verticals, ground_pc_, th_seeds_v_);
            estimate_plane(ground_pc_);

            if (zone_idx == 0 && normal_(2) < uprightness_thr_)
            {
                pcl::PointCloud<PointXYZILID> src_tmp;
                src_tmp = src_wo_verticals;
                src_wo_verticals.clear();

                Eigen::MatrixXf points(src_tmp.points.size(), 3);
                int j = 0;
                for (auto &p:src_tmp.points) {
                    points.row(j++) << p.x, p.y, p.z;
                }
                // ground plane model
                Eigen::VectorXf result = points * normal_;

                for (int r = 0; r < result.rows(); r++) {
                    if (result[r] < th_dist_v_ - d_ && result[r] > -th_dist_v_ - d_) {
                        non_ground_dst.points.push_back(src_tmp[r]);
                        vertical_pc_.points.push_back(src_tmp[r]);
                    } else {
                        src_wo_verticals.points.push_back(src_tmp[r]);
                    }
                }
            }
            else break;
        }
    }

    extract_initial_seeds(zone_idx, src_wo_verticals, ground_pc_);
    estimate_plane(ground_pc_);

    // 2. Region-wise Ground Plane Fitting (R-GPF)
    // : fits the ground plane

    //pointcloud to matrix
    Eigen::MatrixXf points(src_wo_verticals.points.size(), 3);
    int j = 0;
    for (auto &p:src_wo_verticals.points) {
        points.row(j++) << p.x, p.y, p.z;
    }

    for (int i = 0; i < num_iter_; i++) {

        ground_pc_.clear();

        // ground plane model
        Eigen::VectorXf result = points * normal_;
        // threshold filter
        for (int r = 0; r < result.rows(); r++) {
            if (i < num_iter_ - 1) {
                if (result[r] < th_dist_ - d_ ) {
                    ground_pc_.points.push_back(src_wo_verticals[r]);
                }
            } else { // Final stage
                if (result[r] < th_dist_ - d_ ) {
                    dst.points.push_back(src_wo_verticals[r]);
                } else {
                    non_ground_dst.points.push_back(src_wo_verticals[r]);
                }
            }
        }

        if (i < num_iter_ -1) estimate_plane(ground_pc_);
        else estimate_plane(dst);
    }
}

inline
void PatchWorkpp::calc_mean_stdev(std::vector<double> vec, double &mean, double &stdev)
{
    if (vec.size() <= 1) return;

    mean = std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();

    for (int i=0; i<vec.size(); i++) { stdev += (vec.at(i)-mean)*(vec.at(i)-mean); }
    stdev /= vec.size()-1;
    stdev = sqrt(stdev);
}

inline
double PatchWorkpp::xy2theta(const double &x, const double &y) { // 0 ~ 2 * PI
    // if (y >= 0) {
    //     return atan2(y, x); // 1, 2 quadrant
    // } else {
    //     return 2 * M_PI + atan2(y, x);// 3, 4 quadrant
    // }

    double angle = atan2(y, x);
    return angle > 0 ? angle : 2*M_PI+angle;
}

inline
double PatchWorkpp::xy2radius(const double &x, const double &y) {
    return sqrt(pow(x, 2) + pow(y, 2));
}

inline
void PatchWorkpp::callbackCloud(const sensor_msgs::msg::PointCloud2::ConstSharedPtr &cloud_msg)
{
    double time_taken;

    pcl::PointCloud<PointXYZILID> pc_curr;
    pcl::PointCloud<PointXYZILID> pc_ground;
    pcl::PointCloud<PointXYZILID> pc_non_ground;

    pcl::fromROSMsg(*cloud_msg, pc_curr);

    estimate_ground(pc_curr, pc_ground, pc_non_ground, time_taken);
    if(display_time_){
        RCLCPP_INFO_STREAM(rclcpp::get_logger("patchworkpp"), "\033[1;32m" << "Input PointCloud: " << pc_curr.size() << " -> Ground: " << pc_ground.size() <<  "/ NonGround: " << pc_non_ground.size()
            << " (running_time: " << time_taken << " sec)" << "\033[0m");
    }

    pub_cloud->publish(cloud2msg(pc_curr, cloud_msg->header.stamp, cloud_msg->header.frame_id));
    pub_ground->publish(cloud2msg(pc_ground, cloud_msg->header.stamp, cloud_msg->header.frame_id));
    pub_non_ground->publish(cloud2msg(pc_non_ground, cloud_msg->header.stamp, cloud_msg->header.frame_id));
}

sensor_msgs::msg::PointCloud2 PatchWorkpp::cloud2msg(pcl::PointCloud<PointXYZILID> cloud, const rclcpp::Time& stamp, std::string frame_id) {
    sensor_msgs::msg::PointCloud2 cloud_ROS;
    pcl::toROSMsg(cloud, cloud_ROS);
    cloud_ROS.header.stamp = stamp;
    cloud_ROS.header.frame_id = frame_id;
    return cloud_ROS;
}

inline
void PatchWorkpp::pc2czm(const pcl::PointCloud<PointXYZILID> &src, std::vector<Zone> &czm, pcl::PointCloud<PointXYZILID> &cloud_nonground) {

    for (int i=0; i<src.size(); i++) {
        if ((!noise_idxs_.empty()) &&(i == noise_idxs_.front())) {
            noise_idxs_.pop();
            continue;
        }

        PointXYZILID pt = src.points[i];

        double r = xy2radius(pt.x, pt.y);
        if ((r <= max_range_) && (r > min_range_)) {
            double theta = xy2theta(pt.x, pt.y);

            int zone_idx = 0;
            if ( r < min_ranges_[1] ) zone_idx = 0;
            else if ( r < min_ranges_[2] ) zone_idx = 1;
            else if ( r < min_ranges_[3] ) zone_idx = 2;
            else zone_idx = 3;

            int ring_idx = min(static_cast<long>(((r - min_ranges_[zone_idx]) / ring_sizes_[zone_idx])), num_rings_each_zone_[zone_idx] - 1);
            int sector_idx = min(static_cast<long>((theta / sector_sizes_[zone_idx])), num_sectors_each_zone_[zone_idx] - 1);

            czm[zone_idx][ring_idx][sector_idx].points.emplace_back(pt);
        }
        else {
            cloud_nonground.push_back(pt);
        }
    }

    if (verbose_) cout << "[ CZM ] Divides pointcloud into the concentric zone model" << endl;
}

#endif
