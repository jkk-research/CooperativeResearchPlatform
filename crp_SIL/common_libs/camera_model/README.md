# camera_model

**Pinhole Camera Model** class library with related functionalities.

## How to include in your project
1. In your `CMakeLists.txt`, add these (extend with the following, do not replace yours):

    ```cmake
    find_package(catkin REQUIRED COMPONENTS
    roscpp camera_model
    )

    catkin_package(
    CATKIN_DEPENDS camera_model roscpp
    )

    include_directories(
    include
    ${catkin_INCLUDE_DIRS}
    )

    # replace "your project name" with your executable name
    target_link_libraries("your project name"
    ${catkin_LIBRARIES}
    )
    ```
2. In your `package.xml`, add the following:
    ```xml
    <build_depend>roscpp</build_depend>
    <build_depend>camera_model</build_depend>

    <exec_depend>roscpp</exec_depend>
    <exec_depend>camera_model</exec_depend>
    ```

## Available Methods
The available methods and usage examples are in the `camera_model.hpp` header file.