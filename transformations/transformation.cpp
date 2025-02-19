#include "transformation.h"

s21::Matrix4x4 s21::Translate(const Matrix4x4& m, Vector3 v) {
    s21::Matrix4x4 result(m);
    result[3] = m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3];
    return result;
}

s21::Matrix4x4 s21::Rotate(const Matrix4x4 &mat, float angle, Vector3 vec)
{
    const float a = angle;
	const float c = cos(a);
    const float s = sin(a);
    Vector3 axis = normalize(vec);
    Vector3 temp ((1.0f - c) * axis);
    
    Matrix4x4 rotation_mat;
    rotation_mat[0].x = c + temp.x * axis.x;
    rotation_mat[0].y = temp.x * axis.y + s * axis.z;
    rotation_mat[0].z = temp.x * axis.z - s * axis.y;

    rotation_mat[1].x = temp.y * axis.x - s * axis.z;
    rotation_mat[1].y = c + temp.y * axis.y;
    rotation_mat[1].z = temp.y * axis.z + s * axis.x;

    rotation_mat[2].x = temp.z * axis.x + s * axis.y;
    rotation_mat[2].y = temp.z * axis.y - s * axis.x;
    rotation_mat[2].z = c + temp.z * axis.z;

    Matrix4x4 result;
    result[0] = mat[0] * rotation_mat[0][0] + mat[1] * rotation_mat[0][1] + mat[2] * rotation_mat[0][2];
	result[1] = mat[0] * rotation_mat[1][0] + mat[1] * rotation_mat[1][1] + mat[2] * rotation_mat[1][2];
	result[2] = mat[0] * rotation_mat[2][0] + mat[1] * rotation_mat[2][1] + mat[2] * rotation_mat[2][2];
	result[3] = mat[3];

    return result;
}

s21::Matrix4x4 s21::Scale(const Matrix4x4& mat, Vector3 scale) {
    s21::Matrix4x4 result;
    result[0] = scale.x * mat[0];
    result[1] = scale.y * mat[1];
    result[2] = scale.z * mat[2];
    result[3] = mat[3];
    return result;
}