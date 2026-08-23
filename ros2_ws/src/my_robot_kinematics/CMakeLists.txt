cmake_minimum_required(VERSION 3.20)
project(my_robot_kinematics)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(Eigen3 REQUIRED)

# 1. Build a pure C++ Shared Library
add_library(kinematics_engine SHARED src/ur5_kinematics.cpp)

# 2. Link Eigen3
target_include_directories(kinematics_engine PUBLIC
  ${EIGEN3_INCLUDE_DIRS}
)

# 3. Install the library
install(TARGETS kinematics_engine
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()