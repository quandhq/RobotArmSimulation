#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <Eigen/Dense>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/joint_state.hpp"

#include "my_robot_kinematics/ur5_kinematics.hpp"

class UR5KinematicsNode : public rclcpp::Node {
private:
    rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr subscription_;
    std::vector<Eigen::Matrix<double, 6, 1>> S_;
    Eigen::Matrix4d M_;

    void jointStateCallback(const sensor_msgs::msg::JointState::SharedPtr msg) {
        // Safety check: Ensure message data arrays match
        if (msg->name.size() != msg->position.size()) {
            RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 2000,
                                 "JointState message size mismatch between names and positions!");
            return;
        }

        // Build a temporary O(1) hash map lookup from the incoming message
        std::unordered_map<std::string, double> name_to_pos;
        for (size_t i = 0; i < msg->name.size(); ++i) {
            name_to_pos[msg->name[i]] = msg->position[i];
        }

        // Expected physical sequential order for the UR5 arm
        const std::vector<std::string> target_joints = {
            "shoulder_pan_joint", 
            "shoulder_lift_joint", 
            "elbow_joint",
            "wrist_1_joint", 
            "wrist_2_joint", 
            "wrist_3_joint"
        };

        Eigen::Matrix<double, 6, 1> theta;
        for (int i = 0; i < 6; ++i) {
            auto it = name_to_pos.find(target_joints[i]);
            if (it != name_to_pos.end()) {
                theta[i] = it->second;
            } else {
                RCLCPP_WARN_THROTTLE(this->get_logger(), *this->get_clock(), 2000,
                                     "Waiting for all UR5 joints... Missing: %s", target_joints[i].c_str());
                return;
            }
        }

        // Execute the Product of Exponentials chain: T = e^[S1]θ1 * e^[S2]θ2 ... * e^[S6]θ6 * M
        Eigen::Matrix4d T = Eigen::Matrix4d::Identity();
        for (int i = 0; i < 6; ++i) {
            T = T * MatrixExp6(S_[i], theta[i]);
        }
        T = T * M_;

        // Extract final end-effector position vector (p) from the 4x4 transformation matrix
        Eigen::Vector3d p = T.block<3, 1>(0, 3);

        RCLCPP_INFO_THROTTLE(this->get_logger(), *this->get_clock(), 1000,
                             "End-Effector Position -> X: %.4f | Y: %.4f | Z: %.4f",
                             p[0], p[1], p[2]);
    }

    void initializeUR5Kinematics() {
        // Define the 6 screw axes for the UR5 arm (Standard configuration from Lynch & Park)
        S_.resize(6);
        S_[0] << 0, 0, 1, 0, 0, 0;          // Joint 1 (Base rotation)
        S_[1] << 0, 1, 0, -0.089, 0, 0;     // Joint 2 (Shoulder pitch)
        S_[2] << 0, 1, 0, -0.089, 0, 0.425; // Joint 3 (Elbow pitch)
        S_[3] << 0, 1, 0, -0.089, 0, 0.817; // Joint 4 (Wrist 1)
        S_[4] << 0, 0, -1, -0.094, 0.817, 0;// Joint 5 (Wrist 2)
        S_[5] << 0, 1, 0, -0.089, 0, 0.817; // Joint 6 (Wrist 3)

        // Define the Home Configuration Matrix M
        M_ = Eigen::Matrix4d::Identity();
        M_(0, 3) = 0.817; 
        M_(1, 3) = 0.191; 
        M_(2, 3) = 0.174; 
    }

public:
    UR5KinematicsNode() : Node("ur5_kinematics_node") {
        // 1. Initialize structural kinematic parameters (Screw axes & Home Matrix M)
        initializeUR5Kinematics();

        // 2. Setup subscription to Gazebo's joint states topic
        subscription_ = this->create_subscription<sensor_msgs::msg::JointState>(
            "/joint_states", 10,
            std::bind(&UR5KinematicsNode::jointStateCallback, this, std::placeholders::_1));
        
        RCLCPP_INFO(this->get_logger(), "UR5 Kinematics Node initialized and listening to /joint_states.");
    }
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<UR5KinematicsNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}