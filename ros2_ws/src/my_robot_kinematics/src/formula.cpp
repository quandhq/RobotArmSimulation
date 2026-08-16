#include <Eigen>

Eigen::Matrix3d createSkewSymmetricMatrix(Eigen::Vector3d& angVeloVec)
{
    double omega_x = angVeloVec[0];
    double omega_y = angVeloVec[1];
    double omega_z = angVeloVec[2];
    Eigen::Matrix3d skewSymMatr{};
    shewSymMatr << 0, -omega_z, omega_y,
                    omega_z, 0, -omega_x,
                    -omega_y, omega_x, 0;
    return skewSymMatr;
}