#pragma once
#include <cmath>
#include <iostream>
namespace s21 {
struct Vector2 {
  Vector2() : x(0.0f), y(0.0f) {}
  Vector2(float x_, float y_) : x(x_), y(y_) {}
  Vector2(const Vector2 &other) : x(other.x), y(other.y) {}
  float x, y;
};

struct Vector3 {
  Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
  Vector3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
  Vector3(const Vector3 &other) : x(other.x), y(other.y), z(other.z) {}
  float length() const { return std::sqrt(x * x + y * y + z * z); }
  const float &operator[](int n) const {
    switch (n) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    default:
      throw std::out_of_range("Index out of range");
    }
  }
  float x, y, z;
};

struct Vector4 {
  Vector4() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
  Vector4(float x_, float y_, float z_, float w_)
      : x(x_), y(y_), z(z_), w(w_) {}
  Vector4(const Vector4 &other)
      : x(other.x), y(other.y), z(other.z), w(other.w) {}
  const float &operator[](int n) const { // Константная версия
    switch (n) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    case 3:
      return w;
    default:
      throw std::out_of_range("Index out of range");
    }
  }

  float &operator[](int n) {
    switch (n) {
    case 0:
      return x;
    case 1:
      return y;
    case 2:
      return z;
    case 3:
      return w;
    default:
      throw std::out_of_range("Index out of range");
    }
  }

  float x, y, z, w;
};

inline Vector4 operator*(float scalar, const Vector4 &vec) {
  return Vector4(vec.x * scalar, vec.y * scalar, vec.z * scalar,
                 vec.w * scalar);
}
inline Vector4 operator*(const Vector4 &vec, float scalar) {
  return Vector4(vec.x * scalar, vec.y * scalar, vec.z * scalar,
                 vec.w * scalar);
}

inline Vector4 operator+(const Vector4 &vec, const Vector4 &other) {
  return Vector4(vec.x + other.x, vec.y + other.y, vec.z + other.z,
                 vec.w + other.w);
}

inline Vector3 normalize(const Vector3 &vec) {
  float len = vec.length();
  return Vector3(vec.x / len, vec.y / len, vec.z / len);
}

inline Vector3 operator*(float scalar, const Vector3 &vec) {
  return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}
class Matrix4x4 {
public:
  Matrix4x4(const Matrix4x4 &other) {
    mat[0] = other.mat[0];
    mat[1] = other.mat[1];
    mat[2] = other.mat[2];
    mat[3] = other.mat[3];
  }
  Matrix4x4() {
    mat[0] = Vector4(1, 0, 0, 0);
    mat[1] = Vector4(0, 1, 0, 0);
    mat[2] = Vector4(0, 0, 1, 0);
    mat[3] = Vector4(0, 0, 0, 1);
  }

  Vector4 &operator[](int row) { return mat[row]; }

  Matrix4x4 operator*(const Matrix4x4 &other) const {
    Matrix4x4 result;

    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        result[i][j] = 0;
        for (int k = 0; k < 4; ++k) {
          result[i][j] += mat[i][k] * other[k][j]; // Суммируем произведения
        }
      }
    }

    return result;
  }

  const Vector4 &operator[](int row) const { return mat[row]; }

  const float *GetData() const { return &mat[0].x; }
  Vector4 mat[4];
};
} // namespace s21
