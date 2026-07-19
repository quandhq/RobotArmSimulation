#!/bin/bash
# ==============================================================================
# ROS 2 Lyrical Luth Provisioning Script (Ubuntu 26.04)
# ==============================================================================

# Fail fast: Exit immediately if any command returns a non-zero status
set -e
# Ensure pipeline failures are caught
set -o pipefail

echo "[*] Setting up locale..."
sudo apt update && sudo apt install -y locales
sudo locale-gen en_US en_US.UTF-8
sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
export LANG=en_US.UTF-8

echo "[*] Enabling universe repository..."
sudo apt install -y software-properties-common
sudo add-apt-repository universe -y

echo "[*] Adding ROS 2 GPG key..."
sudo apt update && sudo apt install -y curl
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg

echo "[*] Adding ROS 2 repository to sources list..."
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

echo "[*] Installing ROS 2 Lyrical Luth (Desktop)..."
sudo apt update
sudo apt install -y ros-lyrical-desktop

echo "[*] Installing build tools (colcon)..."
sudo apt install -y python3-colcon-common-extensions

echo "[*] Provisioning Complete. Source the setup script in your .bashrc:"
echo "    echo 'source /opt/ros/lyrical/setup.bash' >> ~/.bashrc"