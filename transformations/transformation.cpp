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

  s21::Matrix4x4 Rotate;
  Rotate[0][0] = c + temp[0] * axis[0];
  Rotate[0][1] = temp[0] * axis[1] + s * axis[2];
  Rotate[0][2] = temp[0] * axis[2] - s * axis[1];

  Rotate[1][0] = temp[1] * axis[0] - s * axis[2];
  Rotate[1][1] = c + temp[1] * axis[1];
  Rotate[1][2] = temp[1] * axis[2] + s * axis[0];

  Rotate[2][0] = temp[2] * axis[0] + s * axis[1];
  Rotate[2][1] = temp[2] * axis[1] - s * axis[0];
  Rotate[2][2] = c + temp[2] * axis[2];

  s21::Matrix4x4 Result;
  Result[0] =
      mat[0] * Rotate[0][0] + mat[1] * Rotate[0][1] + mat[2] * Rotate[0][2];
  Result[1] =
      mat[0] * Rotate[1][0] + mat[1] * Rotate[1][1] + mat[2] * Rotate[1][2];
  Result[2] =
      mat[0] * Rotate[2][0] + mat[1] * Rotate[2][1] + mat[2] * Rotate[2][2];
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