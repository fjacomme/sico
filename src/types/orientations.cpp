#include "sico/types/orientations.hpp"

#include "sico/types/angles.hpp"
#include "sico/types/linear_algebra.hpp"

using namespace sico;

void internal::ori_to_quat(
    radians yaw, radians pitch, radians roll, double& x, double& y, double& z, double& w)
{
    auto const xR = -raw(roll);
    auto const yR = -raw(pitch);
    auto const zR = -raw(yaw);
#ifdef SICO_USE_EIGEN
    quaternion const quat = Eigen::AngleAxisd(xR, vector3d::UnitX())
        * Eigen::AngleAxisd(yR, vector3d::UnitY()) * Eigen::AngleAxisd(zR, vector3d::UnitZ());
    x = quat.x();
    y = quat.y();
    z = quat.z();
    w = quat.w();
    
#else
    auto const quat = from_euler(xR, yR, zR);

    x = quat[0];
    y = quat[1];
    z = quat[2];
    w = quat[3];
#endif
}

void internal::quat_to_ori(
    double x, double y, double z, double w, radians& yaw, radians& pitch, radians& roll)
{
#ifdef SICO_USE_EIGEN
    quaternion const quat(w, x, y, z);

    auto const rot = quat.toRotationMatrix().eulerAngles(0, 1, 2);

    yaw   = radians(-rot[0]);
    pitch = radians(-rot[1]);
    roll  = radians(-rot[2]);
#else
    quaternion const quat = { x, y, z, w };

    auto const mat = rot_mat(quat);
    double     xR, yR, zR;
    to_euler(mat, xR, yR, zR);
    yaw   = radians(-zR);
    pitch = radians(-yR);
    roll  = radians(-xR);
#endif
}