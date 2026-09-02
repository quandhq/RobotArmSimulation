#include <cmath>
#include <Eigen/Dense>

Eigen::Matrix3d createSkewSymmetricMatrix(const Eigen::Vector3d& vec)
{
    double omega_x = vec[0];
    double omega_y = vec[1];
    double omega_z = vec[2];
    Eigen::Matrix3d skewSymMatr{};
    skewSymMatr << 0, -omega_z, omega_y,
                    omega_z, 0, -omega_x,
                    -omega_y, omega_x, 0;
    return skewSymMatr;
}

Eigen::Matrix3d rodriguesRotation(const Eigen::Vector3d& vec, double theta) {
    Eigen::Matrix3d skew_symmetric_matrix = createSkewSymmetricMatrix(vec);
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

//Create Lie algebra of Special Euclidean Group se3 from Skew Axis vector
Eigen::Matrix4d screwAxisToSe3(const Eigen::Vector<double, 6>& S) {
    Eigen::Matrix4d se3 = Eigen::Matrix4d::Zero();
    Eigen::Vector3d rotaVeloVec = S.head<3>();
    Eigen::Vector3d lineaVeloVec = S.tail<3>();
    Eigen::Matrix3d skewSymMatrOfRotaVeloVec = createSkewSymmetricMatrix(rotaVeloVec);
    se3.block<3,3>(0,0) = skewSymMatrOfRotaVeloVec;
    se3.block<3,1>(0,3) = lineaVeloVec;
    return se3;
}

Eigen::Vector3d calculateTranslation(const Eigen::Vector3d& linVeloVec,
    const Eigen::Vector3d& rotaVeloVec, double theta) {
    const Eigen::Matrix3d skewSymMatrOfRotaVeloVec = createSkewSymmetricMatrix(rotaVeloVec);
    Eigen::Vector3d transVec = (theta*Eigen::Matrix3d::Identity() + (1-std::cos(theta))*skewSymMatrOfRotaVeloVec 
        + (theta - std::sin(theta))*skewSymMatrOfRotaVeloVec*skewSymMatrOfRotaVeloVec)*linVeloVec;
    return transVec;
}

Eigen::Matrix4d MatrixExp6(const Eigen::Vector<double, 6>& S, double theta) {
    Eigen::Vector3d rotaVeloVec = S.head<3>();
    Eigen::Vector3d linVeloVec = S.tail<3>();
    Eigen::Matrix3d RotaMatrix_R = rodriguesRotation(rotaVeloVec, theta);
    Eigen::Vector3d transVec_P = calculateTranslation(linVeloVec, rotaVeloVec, theta);
    Eigen::Matrix4d TranformMatrix_T = createTransformMatrix(RotaMatrix_R, transVec_P);
    return TranformMatrix_T;
}