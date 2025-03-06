#include "transformation.h"

s21::Matrix4x4 s21::Translate(const Matrix4x4 &m, Vector3 v) {
    s21::Matrix4x4 result(m);
    result[3] = m[0] * v.x + m[1] * v.y + m[2] * v.z + m[3];
    return result;
}

s21::Matrix4x4 s21::Rotate(const Matrix4x4 &mat, float angle, Vector3 vec) {
    float const a = angle;
    float const c = cos(a);
    float const s = sin(a);

    Vector3 axis(normalize(vec));
    Vector3 temp((1.0 - c) * axis);

    s21::Matrix4x4 rotate;
    rotate[0][0] = c + temp[0] * axis[0];
    rotate[0][1] = temp[0] * axis[1] + s * axis[2];
    rotate[0][2] = temp[0] * axis[2] - s * axis[1];

    rotate[1][0] = temp[1] * axis[0] - s * axis[2];
    rotate[1][1] = c + temp[1] * axis[1];
    rotate[1][2] = temp[1] * axis[2] + s * axis[0];

    rotate[2][0] = temp[2] * axis[0] + s * axis[1];
    rotate[2][1] = temp[2] * axis[1] - s * axis[0];
    rotate[2][2] = c + temp[2] * axis[2];

    s21::Matrix4x4 Result;
    Result[0] = mat[0] * rotate[0][0] + mat[1] * rotate[0][1] + mat[2] * rotate[0][2];
    Result[1] = mat[0] * rotate[1][0] + mat[1] * rotate[1][1] + mat[2] * rotate[1][2];
    Result[2] = mat[0] * rotate[2][0] + mat[1] * rotate[2][1] + mat[2] * rotate[2][2];
    Result[3] = mat[3];
    return Result;
}

s21::Matrix4x4 s21::Scale(const Matrix4x4 &mat, Vector3 scale) {
    s21::Matrix4x4 result;
    result[0] = scale.x * mat[0];
    result[1] = scale.y * mat[1];
    result[2] = scale.z * mat[2];
    result[3] = mat[3];
    return result;
}

s21::Matrix4x4 s21::LookAt(const Vector3 &eye, const Vector3 &center, const Vector3 &up) {
    const Vector3 f(normalize(center - eye));  // vector of direction of camera
    const Vector3 s(normalize(f.cross(up)));   // right direction of camera
    const Vector3 u(s.cross(f));               // vector UP of camera

    s21::Matrix4x4 result;
    result[0][0] = s.x;
    result[1][0] = s.y;
    result[2][0] = s.z;
    result[0][1] = u.x;
    result[1][1] = u.y;
    result[2][1] = u.z;
    result[0][2] = f.x;
    result[1][2] = f.y;
    result[2][2] = f.z;
    result[3][0] = -s.dot(eye);
    result[3][1] = -u.dot(eye);
    result[3][2] = -f.dot(eye);
    return result;
}

s21::Matrix4x4 s21::perspective(float fov, float aspect, float far, float near) {
    float tanHalfFov = tan(fov / 2.0f);

    Matrix4x4 result;

    result[0][0] = 1.0f / (aspect * tanHalfFov);
    result[1][1] = 1.0f / tanHalfFov;
    result[2][2] = -(far + near) / (far - near);
    result[2][3] = -1.0f;
    result[3][2] = -(2.0f * far * near) / (far - near);

    return result;
}
