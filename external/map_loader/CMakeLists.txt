cmake_minimum_required(VERSION 3.14)
project(map_loader)

find_package(autoware_cmake REQUIRED)
find_package(tier4_map_msgs REQUIRED)
autoware_package()


ament_auto_add_library(lanelet2_map_visualization_node SHARED
  src/lanelet2_map_loader/lanelet2_map_visualization_node.cpp
)

rclcpp_components_register_node(lanelet2_map_visualization_node
  PLUGIN "Lanelet2MapVisualizationNode"
  EXECUTABLE lanelet2_map_visualization
)


install(PROGRAMS
  script/map_hash_generator
  DESTINATION lib/${PROJECT_NAME}
)

ament_auto_package(INSTALL_TO_SHARE
  launch
  config
)
