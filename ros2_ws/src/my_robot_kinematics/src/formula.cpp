#include <cmath>
#include <Eigen/Dense>

Eigen::Matrix3d createSkewSymmetricMatrix(const Eigen::Vector3d& angVeloVec)
{
    double omega_x = angVeloVec[0];
    double omega_y = angVeloVec[1];
    double omega_z = angVeloVec[2];
    Eigen::Matrix3d skewSymMatr{};
    skewSymMatr << 0, -omega_z, omega_y,
                    omega_z, 0, -omega_x,
                    -omega_y, omega_x, 0;
    return skewSymMatr;
}

Eigen::Matrix3d rodriguesRotation(const Eigen::Vector3d& omega, double theta) {
    Eigen::Matrix3d skew_symmetric_matrix = createSkewSymmetricMatrix(omega);
    Eigen::Matrix3d I = Eigen::Matrix3d::Identity();
    Eigen::Matrix3d rotation_matrix = I + std::sin(theta)*skew_symmetric_matrix + (1-std::cos(theta))*skew_symmetric_matrix*skew_symmetric_matrix;
    return rotation_matrix;
}

Eigen::Matrix4d createTransformMatrix(const Eigen::Matrix3d& R, const Eigen::Vector3d& p) {
    // Write your implementation here
    Eigen::Matrix4d T = Eigen::Matrix4d::Identity();
    T.block<3,3>(0,0) = R;
    T.block<3,1>(0,3) = p;
    return T;
}