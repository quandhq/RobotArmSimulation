ROS 2 Workspace & Environment Cheatsheet

Here is the standard workflow for setting up a new robotics project.

1. Source the "Underlay" (The Core ROS 2 Installation)

Before you can use any ROS 2 commands (like ros2 or colcon), you have to load the core ROS 2 environment into your terminal.

# Replace 'humble' with your specific ROS 2 version (e.g., iron, jazzy, foxy)
source /opt/ros/humble/setup.bash


Tip: Most developers add this line to their ~/.bashrc file so it runs automatically every time they open a new terminal.

2. Create your Workspace (The Project Folder)

A ROS 2 workspace is just a directory with a specific structure. All your custom code goes into a subfolder named src.

# Create the workspace and the 'src' folder at the same time, in the current folder
mkdir -p ros2_ws/src

# Move into the workspace
cd ros2_ws


3. Create a Package

You don't put code directly into src. Instead, you create Packages. A package is a self-contained unit of code (like your Ur5Kinematics node).

# Move into the src directory
cd ros2_ws/src

# Create a C++ package (ament_cmake)
# Syntax: ros2 pkg create --build-type ament_cmake <package_name> --dependencies <deps>
ros2 pkg create --build-type ament_cmake my_robot_kinematics --dependencies rclcpp sensor_msgs Eigen3


Note: If you were writing Python, you would use --build-type ament_python.

4. Build the Workspace

Once you have written your C++ code (like saving ur5_kinematics.cpp into your package's src folder and updating CMakeLists.txt), you need to compile it.

# ALWAYS build from the root of your workspace, never from inside 'src'
cd ros2_ws

# Compile all packages
colcon build

# Or, to compile just one specific package:
colcon build --packages-select my_robot_kinematics


5. Source the "Overlay" (Your Custom Workspace)

After building, colcon creates several new folders (build, install, log).
Even though you built your code, ROS 2 doesn't know it exists yet. You have to source your workspace's specific setup file so ROS can find your newly compiled nodes.

# Make sure you are in the workspace root
cd ros2_ws

# Source your custom environment
source install/setup.bash


6. Run Your Node!

Now that both the underlay (Core ROS) and overlay (Your Code) are sourced, you can run your robot!

# Syntax: ros2 run <package_name> <executable_name>
ros2 run my_robot_kinematics kinematics_node


💡 Summary of the "Double Source" Concept

The most common mistake in ROS 2 is forgetting to source. Remember this rule:

source /opt/ros/... tells the terminal where ROS 2 is.

source install/setup.bash tells ROS 2 where your code is.