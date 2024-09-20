# Lanelet handler

A package for handling lanelet2 maps. Based on the autoware [map_loader](https://github.com/autowarefoundation/autoware.universe/tree/main/map/map_loader) package.

## Lanelet file handling

The lanelet_file_loader node can be used to load a lanelet2 map from a .osm file and publish the binary data to a topic.

### Usage:

```
ros2 run lanelet_handler lanelet_file_loader --ros-args -p map_file_path:=/path/to/map_file.osm -p map_output_topic:=/output/topic
```
or by using the launcher:
```
ros2 launch lanelet_handler laneletFileLoader.launch.py
```

### Parameters
| Name             | Type   | Default value                                   |
|------------------|--------|-------------------------------------------------|
| map_file_path    | string | ""                                              |
| map_output_topic | string | "/map/global_static_map_from_file/lanelet2_map" |

