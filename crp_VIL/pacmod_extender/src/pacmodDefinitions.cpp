#include "pacmod_extender/pacmodDefinitions.hpp"


pacmod3_msgs::msg::LinearAccelRpt PacmodDefinitions::decodeLinAccel(const can_msgs::msg::Frame::SharedPtr msg)
{
    if (msg->id != M_LIN_ACCEL_ID)
        throw std::invalid_argument("Invalid message ID for linear acceleration message");

    pacmod3_msgs::msg::LinearAccelRpt linAccel;
    linAccel.header.stamp = msg->header.stamp;
    linAccel.header.frame_id = msg->header.frame_id;
    linAccel.lateral_new_data_rx = msg->data[0] & 0x1;
    linAccel.longitudinal_new_data_rx = (msg->data[0] >> 1) & 0x1;
    linAccel.vertical_new_data_rx = (msg->data[0] >> 2) & 0x1;
    linAccel.lateral_valid = (msg->data[0] >> 3) & 0x1;
    linAccel.longitudinal_valid = (msg->data[0] >> 4) & 0x1;
    linAccel.vertical_valid = (msg->data[0] >> 5) & 0x1;

    int16_t accelLatRaw = msg->data[1] << 8 | msg->data[2];
    linAccel.lateral_accel = accelLatRaw * 0.01;

    int16_t accelLonRaw = msg->data[3] << 8 | msg->data[4];
    linAccel.longitudinal_accel = accelLonRaw * 0.01;

    int16_t accelVertRaw = msg->data[5] << 8 | msg->data[6];
    linAccel.vertical_accel = accelVertRaw * 0.01;

    return linAccel;
}
