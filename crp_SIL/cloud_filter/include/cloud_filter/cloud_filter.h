#pragma once

//ros
#include "rclcpp/rclcpp.hpp"
#include "./point.h"
#include "PolyfitBoost.hpp"



namespace params{
    extern std::string input_topic;
    extern bool filter_a,filter_i,filter_re,filter_ra,filter_z;
    extern bool isRoad,ring_based_histogram;
    extern float mult_a,mult_i,mult_ra,mult_re,mult_z;
    extern float limit_a,limit_i,limit_ra,limit_re,limit_z;
    extern double min_x,min_y,min_z;
    extern double max_x,max_y,max_z;
    extern int start_index,end_index;
    extern double clus_thres,cut_p,bin_size;
    extern double interval;
    extern int point_number,channel_number;
};

