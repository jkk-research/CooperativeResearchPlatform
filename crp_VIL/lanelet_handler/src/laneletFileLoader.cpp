#include "lanelet_handler/laneletFileLoader.hpp"


crp::vil::LaneletFileLoader::LaneletFileLoader() : Node("lanelet_file_loader")
{
    std::string mapFilePath;
    std::string mapOutputTopic;
    this->declare_parameter<std::string>("map_file_path",    "");
    this->declare_parameter<std::string>("map_output_topic", "");
    this->declare_parameter<std::string>("map_frame_id",     "map");

    this->get_parameter<std::string>("map_file_path",    mapFilePath);
    this->get_parameter<std::string>("map_output_topic", mapOutputTopic);
    this->get_parameter<std::string>("map_frame_id",     m_mapFrameId);

    if (mapFilePath.empty()) {
        RCLCPP_ERROR(this->get_logger(), "map_file_path is empty");
        return;
    }
    else if (!std::filesystem::exists(mapFilePath)) {
        RCLCPP_ERROR(this->get_logger(), "map_file_path does not exist: %s", mapFilePath.c_str());
        return;
    }

    m_pub_laneletMap_ = this->create_publisher<autoware_map_msgs::msg::LaneletMapBin>(
        mapOutputTopic, rclcpp::QoS{1}.transient_local());

    RCLCPP_INFO(this->get_logger(), "lanelet_file_loader has been started");

    publishLaneletMap(mapFilePath);
}


autoware_map_msgs::msg::LaneletMapBin crp::vil::LaneletFileLoader::createMapBinMsg(const lanelet::LaneletMapPtr map, const std::string & lanelet2Filename, const rclcpp::Time & now)
{
  std::string formatVersion{};
  std::string map_version{};
  lanelet::io_handlers::AutowareOsmParser::parseVersions(
    lanelet2Filename, &formatVersion, &map_version);

  autoware_map_msgs::msg::LaneletMapBin mapBinMsg;
  mapBinMsg.header.stamp = now;
  mapBinMsg.header.frame_id = m_mapFrameId;
  mapBinMsg.version_map_format = formatVersion;
  mapBinMsg.version_map = map_version;
  lanelet::utils::conversion::toBinMsg(map, &mapBinMsg);

  return mapBinMsg;
}


void crp::vil::LaneletFileLoader::publishLaneletMap(const std::string& lanelet2FilePath)
{
    lanelet::LaneletMapPtr laneletMap = loadLaneletMap(lanelet2FilePath);
    const auto mapBinMsg = crp::vil::LaneletFileLoader::createMapBinMsg(laneletMap, lanelet2FilePath, now());
    m_pub_laneletMap_->publish(mapBinMsg);
}


lanelet::LaneletMapPtr crp::vil::LaneletFileLoader::loadLaneletMap(const std::string& lanelet2FilePath)
{
    // load lanelet file
    lanelet::ErrorMessages errors{};
    lanelet::projection::MGRSProjector projector{};
    lanelet::LaneletMapPtr map = lanelet::load(lanelet2FilePath, projector, &errors);

    // overwrite with local_x, local_y if they exist
    bool hasLocal = false;
    for (lanelet::Point3d point : map->pointLayer) {
        if (point.hasAttribute("local_x")) {
            point.x() = point.attribute("local_x").asDouble().value();
            hasLocal = true;
        }
        if (point.hasAttribute("local_y")) {
            point.y() = point.attribute("local_y").asDouble().value();
            hasLocal = true;
        }
    }

    if (hasLocal) {
        // realign lanelet borders using updated points
        for (lanelet::Lanelet lanelet : map->laneletLayer) {
            auto left = lanelet.leftBound();
            auto right = lanelet.rightBound();
            std::tie(left, right) = lanelet::geometry::align(left, right);
            lanelet.setLeftBound(left);
            lanelet.setRightBound(right);
        }
    }

    lanelet::utils::overwriteLaneletsCenterline(map, 1, false);

    return map;
}


int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<crp::vil::LaneletFileLoader>());
    rclcpp::shutdown();
    return 0;
}
