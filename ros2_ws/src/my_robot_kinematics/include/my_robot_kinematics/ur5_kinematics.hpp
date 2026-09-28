#ifndef UR5_KINEMATICS_HPP
#define UR5_KINEMATICS_HPP
#include <Eigen/Dense>

// Function declarations (The public interface contract)
Eigen::Matrix3d createSkewSymmetricMatrix(const Eigen::Vector3d& vec);
Eigen::Matrix3d rodriguesRotation(const Eigen::Vector3d& vec, double theta);
Eigen::Matrix4d createTransformMatrix(const Eigen::Matrix3d& R, const Eigen::Vector3d& p);
Eigen::Matrix4d screwAxisToSe3(const Eigen::Vector<double, 6>& S);
Eigen::Vector3d calculateTranslation(const Eigen::Vector3d& linVeloVec,
                                     const Eigen::Vector3d& rotaVeloVec, double theta);
Eigen::Matrix4d MatrixExp6(const Eigen::Vector<double, 6>& S, double theta);

#endif // UR5_KINEMATICS_HPP