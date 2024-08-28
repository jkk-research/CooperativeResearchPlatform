#ifndef CRP_VIL_LANELET_FILE_LOADER_LANELETHANDLER_HPP
#define CRP_VIL_LANELET_FILE_LOADER_LANELETHANDLER_HPP


#include <filesystem>

#include <rclcpp/rclcpp.hpp>
#include <lanelet2_core/LaneletMap.h>
#include <autoware_map_msgs/msg/lanelet_map_bin.hpp>
#include <lanelet2_io/Io.h>
#include <lanelet2_extension/projection/mgrs_projector.hpp>
#include <lanelet2_extension/utility/utilities.hpp>
#include <autoware_lanelet2_extension/io/autoware_osm_parser.hpp>
#include <autoware_lanelet2_extension/utility/message_conversion.hpp>
#include <lanelet2_core/geometry/LineString.h>


namespace crp
{
namespace vil
{

class LaneletFileLoader : public rclcpp::Node
{
public:
    LaneletFileLoader();

private:
    autoware_map_msgs::msg::LaneletMapBin createMapBinMsg(
        const lanelet::LaneletMapPtr map,
        const std::string & lanelet2Filename,
        const rclcpp::Time & now);
    lanelet::LaneletMapPtr loadLaneletMap(const std::string& lanelet2FilePath);
    void publishLaneletMap(const std::string& lanelet2FilePath);

    rclcpp::Publisher<autoware_map_msgs::msg::LaneletMapBin>::SharedPtr m_pub_laneletMap_;
};

} // namespace vil
} // namespace crp
#endif //CRP_VIL_LANELET_FILE_LOADER_LANELETHANDLER_HPP