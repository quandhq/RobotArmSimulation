cmake_minimum_required(VERSION 3.16)
project(my_robot_kinematics)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find all required dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(Eigen3 REQUIRED)

# 1. Build your core kinematics shared library from the implementation file
add_library(kinematics_engine SHARED
  src/ur5_kinematics.cpp
)

# Explicitly point to the local include directory so the library can find its own headers
target_include_directories(kinematics_engine PUBLIC
  $
  $
)
# Add a direct include path for internal compilation safety
target_include_directories(kinematics_engine PRIVATE include)

target_link_libraries(kinematics_engine PUBLIC Eigen3::Eigen)

# 2. Build the ROS 2 node executable using Modern CMake linking
add_executable(ur5_node src/ur5_node.cpp)

target_include_directories(ur5_node PUBLIC include)

target_link_libraries(ur5_node
  PRIVATE
    kinematics_engine
    Eigen3::Eigen
    rclcpp::rclcpp
    sensor_msgs::sensor_msgs__rosidl_typesupport_cpp
)

# 3. Install targets and headers for colcon workspace management
install(TARGETS
  kinematics_engine
  ur5_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY include/
  DESTINATION include
)

ament_package()