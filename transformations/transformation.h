#pragma once
#include "mat4.h"

namespace s21 {
    Matrix4x4 Translate(const Matrix4x4& mat, Vector3 vec);
    Matrix4x4 Rotate(const Matrix4x4& mat, float angle, Vector3 vec);
    Matrix4x4 Scale(const Matrix4x4& mat, Vector3 vec);
}