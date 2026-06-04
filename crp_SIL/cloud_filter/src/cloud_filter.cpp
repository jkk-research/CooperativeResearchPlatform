#include "rclcpp/rclcpp.hpp"
#include "tf2_ros/buffer.h"
#include "sensor_msgs/msg/point_cloud2.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include <pcl/point_cloud.h>
#include "pcl_conversions/pcl_conversions.h"
#include <rmw/qos_profiles.h>

#include "cloud_filter/cloud_filter.h"

bool ring_based_histogram = true;


class cloud_filter_tmp : public rclcpp::Node
{
public:
    cloud_filter_tmp(): Node("cloud_filter")
    { 
        
        auto sensor_qos = rclcpp::QoS(rclcpp::KeepLast(1),rmw_qos_profile_sensor_data);
        sub = this->create_subscription<sensor_msgs::msg::PointCloud2>("/ground",sensor_qos,std::bind(&cloud_filter_tmp::Callback,this, std::placeholders::_1));


        pub_cloud = this->create_publisher<sensor_msgs::msg::PointCloud2>("/filtered_points",1);
       
        pub_amb_ = this->create_publisher<std_msgs::msg::Float32MultiArray>("/ambient_histogram",1);
        pub_int = this->create_publisher<std_msgs::msg::Float32MultiArray>("/intensity_histogram",1);
        pub_range = this->create_publisher<std_msgs::msg::Float32MultiArray>("/range_histogram",1);
        pub_refl = this->create_publisher<std_msgs::msg::Float32MultiArray>("/reflectivity_histogram",1);
    }
private:

    float max_amb; 
    float max_int;
    float max_rang;
    float max_refl;


    void Callback(const sensor_msgs::msg::PointCloud2::SharedPtr msg)
    {

    
        pcl::PointCloud<ouster::Point>::Ptr input_cloud(new pcl::PointCloud<ouster::Point>);
        pcl::fromROSMsg(*msg,*input_cloud);

        //publish the input cloud
        sensor_msgs::msg::PointCloud2 raw_msg;
        pcl::toROSMsg(*input_cloud,raw_msg);
        //raw_msg.header = msg->header;
        //pub_raw->publish(raw_msg);

        pcl::PointCloud<ouster::Point>::Ptr output_cloud(new pcl::PointCloud<ouster::Point>);
        LimitFilter(input_cloud,output_cloud);
        setHistogramData(input_cloud,max_amb,max_int,max_rang,max_refl);

       

        sensor_msgs::msg::PointCloud2 output_msg;
        pcl::toROSMsg(*output_cloud,output_msg);

       

        output_msg.header = msg->header;
        output_msg.header.stamp = rclcpp::Clock().now();
        //Publish results
        pub_cloud->publish(output_msg);
    }
    void setHistogramData(const pcl::PointCloud<ouster::Point>::ConstPtr cloud,float max_amb,float max_int,float max_rang,float max_refl)
    {

        std_msgs::msg::Float32MultiArray amb_data;
        std_msgs::msg::Float32MultiArray int_data;
        std_msgs::msg::Float32MultiArray range_data;
        std_msgs::msg::Float32MultiArray refl_data;

        amb_data.layout.dim.push_back(std_msgs::msg::MultiArrayDimension());

        amb_data.layout.dim[0].label = "ambient";
        amb_data.layout.dim[0].size = cloud->points.size();
        amb_data.layout.dim[0].stride = cloud->points.size();
        amb_data.layout.data_offset = 0;

        int_data.layout.dim.push_back(std_msgs::msg::MultiArrayDimension());

        int_data.layout.dim[0].label = "intensity";
        int_data.layout.dim[0].size = cloud->points.size();
        int_data.layout.dim[0].stride = cloud->points.size();
        int_data.layout.data_offset = 0;

        range_data.layout.dim.push_back(std_msgs::msg::MultiArrayDimension());

        range_data.layout.dim[0].label = "range";
        range_data.layout.dim[0].size = cloud->points.size();
        range_data.layout.dim[0].stride = cloud->points.size();
        range_data.layout.data_offset = 0;

        refl_data.layout.dim.push_back(std_msgs::msg::MultiArrayDimension());

        refl_data.layout.dim[0].label = "reflectivity";
        refl_data.layout.dim[0].size = cloud->points.size();
        refl_data.layout.dim[0].stride = cloud->points.size();
        refl_data.layout.data_offset = 0;
        

        std::vector<float> amb_vec(cloud->points.size(),0);
        std::vector<float> int_vec(cloud->points.size(),0);
        std::vector<float> refl_vec(cloud->points.size(),0);
        std::vector<float> range_vec(cloud->points.size(),0);

        size_t cloud_size = cloud->points.size();
        for(size_t i=0;i<cloud_size;++i){
            amb_vec[i] = cloud->points[i].ambient/max_amb;
            int_vec[i] = cloud->points[i].intensity/max_int;
            //range_vec[i] = cloud->points[i].range/max_rang;
            refl_vec[i] = cloud->points[i].reflectivity/max_refl;
        }


        amb_data.data = amb_vec;
        int_data.data = int_vec;
        refl_data.data = refl_vec;
        range_data.data = range_vec;

        pub_amb_->publish(amb_data);
        pub_int->publish(int_data);
        pub_refl->publish(refl_data);
        pub_range->publish(range_data);
    }

float distance(const ouster::Point& point1,const ouster::Point& point2){
    float x=point1.x-point2.x;
    float y=point1.y-point2.y;
    return sqrt(pow(x,2)+pow(y,2));
}

float angle(const ouster::Point& point1,const ouster::Point& point2){
    float x=point1.x-point2.x;
    float y=point1.y-point2.y;
    float angle=atan2(y,x);
    if(angle<=0){
        angle=2*M_PI+angle;
    }
    return angle;
}

float get_Min(std::vector<float>& values)
{
    float min = std::numeric_limits<float>::max();

    for(const auto& value : values)
        if(value < min)
            min = value;

    return min;

}
float get_Max(std::vector<float>& values)
{
    float max = std::numeric_limits<float>::min();

    for(const auto& value : values)
        if(value > max)
            max = value;

    return max;

}
// create function that calculates standard deviation for a vector of values
float get_StdDev(std::vector<float>& values)
{
    float sum = 0.0;
    float mean = 0.0;
    float standardDeviation = 0.0;

    for(int i = 0; i < values.size(); ++i)
    {
        sum += values[i];
    }

    mean = sum/values.size();

    for(int i = 0; i < values.size(); ++i)
        standardDeviation += pow(values[i] - mean, 2);

    return sqrt(standardDeviation / values.size());
}

int getBinsize(std::vector<float>& values){
    float min = get_Min(values);
    float max = get_Max(values);
    float std_dev = get_StdDev(values);
    float bin_size = 3.49*std_dev*pow(values.size(),-1.0/3.0);

    if(bin_size < 1.0){
        bin_size=1.0;
    }

    return bin_size;
}

float getMaxValuefromHistogram(std::vector<float> values)
{
    std::sort(values.begin(),values.end());

    float min = get_Min(values);
    float max = get_Max(values);

    int bin_size = getBinsize(values);

    

    std::vector<float> bins;

    for(float i = min;i < max; i += bin_size)
        bins.push_back(i);


    std::vector<int> histogram(bins.size());    
    int bin_stepper = 0;
    if (bins.size() == 0)
        return 0;

    for(const auto& value : values)
    {
        if(value < bins[bin_stepper] && bin_stepper < histogram.size())
            histogram[bin_stepper]++;
        else
            bin_stepper++;
            if(bin_stepper >= bins.size())
                bin_stepper = bins.size()-1;
    }

    int break_i = 0;
    for(int i = 0;i < histogram.size()-1;i++)
    {
        if(histogram[i] < histogram[i+1])
            break_i = i+1;
        else
            break;
    }

    for(int i = break_i;i < histogram.size()-1;i++)
    {
        if(histogram[i] > histogram[i+1])
            break_i = i+1;
        else
            break;
    }

    if(bins.size() == 0)
        return 0;

    if(break_i >= bins.size())
        break_i = bins.size()-1;

    return bins[break_i];
}

void ringFiltering(std::map<int, std::vector<ouster::Point>>& raw_cloud, std::map<int, std::vector<ouster::Point>>& filtered_cloud)
{
    for(const auto& ring : raw_cloud){
       
        float max_ra=0;
        float max_z=0;

        if(ring.second.size() < 10)
            continue;

        std::vector<float> values;

        for(auto point : ring.second)
            values.push_back(point.intensity);

        max_int = getMaxValuefromHistogram(values);
        
        
        values.clear();


        for(auto point : ring.second)
            values.push_back(point.ambient);

        max_amb = getMaxValuefromHistogram(values);

       
        values.clear();

        for(auto point : ring.second)
        {
            values.push_back(point.reflectivity);
        }

        max_refl = getMaxValuefromHistogram(values);
    


        
        for(const auto& point : ring.second)
        {
            if( point.ambient<max_amb && point.reflectivity<max_refl)
                filtered_cloud[point.ring].push_back(point);
                
                
        }        
    
    }
    
}
void cloudSegmentation(const pcl::PointCloud<ouster::Point>::ConstPtr& cloud, std::map<int, std::vector<ouster::Point>>& filtered_cloud)
{

   

    std::vector<float> values;
    for(auto &point : *cloud)
    {
        values.push_back(point.ambient);
    }

    max_amb = getMaxValuefromHistogram(values);

    values.clear();

    for(auto &point : *cloud)
    {
        values.push_back(point.reflectivity);
    }

    max_refl = getMaxValuefromHistogram(values);


    for(auto &point : *cloud)
    {
        if(point.reflectivity<max_refl && point.ambient<max_amb)
            filtered_cloud[point.ring].push_back(point);
    }
    
}
void LimitFilter(const pcl::PointCloud<ouster::Point>::ConstPtr cloud,pcl::PointCloud<ouster::Point>::Ptr output)
{
    

    std::map<int, std::vector<ouster::Point>> temp_cloud, temp;

    // print out the size of the cloud  
    //RCLCPP_INFO(this->get_logger(),"Cloud Size: %d",cloud->points.size());

    //std::vector<pcl::PointCloud<ouster::Point>> temp(300),temp_refl(300),temp_amb(300),temp_cloud(300);
    for(auto point : *cloud)
    {
        temp_cloud[point.ring].push_back(point);
    }

    

    if(ring_based_histogram)
    {
        ringFiltering(temp_cloud,temp);
    }    
    else
    {
      cloudSegmentation(cloud,temp);
    }

    
    ouster::Point null_p;
    null_p.x = 0.0;
    null_p.y = 0.0;
    for(const auto& ring:temp)
    {
        for(const auto& point : ring.second)
        {
            if(distance(point,null_p) < 25 && distance(point,null_p) > 0)
            
            {
                output->points.push_back(point);
            }
        }
    }
    
    
}

    rclcpp::Subscription<sensor_msgs::msg::PointCloud2>::SharedPtr sub;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_cloud;
    rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pub_raw;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr pub_amb_,pub_int,pub_range,pub_refl;
   
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<cloud_filter_tmp>());
    rclcpp::shutdown();
    return 0;
}