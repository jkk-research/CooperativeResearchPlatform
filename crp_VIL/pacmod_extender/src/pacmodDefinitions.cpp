#include "pacmod_extender/pacmodDefinitions.hpp"


pacmod3_msgs::msg::LinearAccelRpt PacmodDefinitions::decodeLinAccel(const can_msgs::msg::Frame::SharedPtr msg)
{
    pacmod3_msgs::msg::LinearAccelRpt linAccel;
    linAccel.header.stamp = msg->header.stamp;
    linAccel.header.frame_id = msg->header.frame_id;
    linAccel.lateral_new_data_rx = msg->data[0] & 0x1;
    linAccel.longitudinal_new_data_rx = (msg->data[0] >> 1) & 0x1;
    linAccel.vertical_new_data_rx = (msg->data[0] >> 2) & 0x1;
    linAccel.lateral_valid = (msg->data[0] >> 3) & 0x1;
    linAccel.longitudinal_valid = (msg->data[0] >> 4) & 0x1;
    linAccel.vertical_valid = (msg->data[0] >> 5) & 0x1;

    int16_t accelLat_raw = msg->data[1] << 8 | msg->data[2];
    linAccel.lateral_accel = accelLat_raw * 0.01;

    int16_t accelLon_raw = msg->data[3] << 8 | msg->data[4];
    linAccel.longitudinal_accel = accelLon_raw * 0.01;

    int16_t accelVert_raw = msg->data[5] << 8 | msg->data[6];
    linAccel.vertical_accel = accelVert_raw * 0.01;

    return linAccel;
}