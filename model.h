#pragma once

#include <fstream>
#include <vector>

#include "transformations/transformation.h"
namespace s21 {
struct Vertex {
    Vector3 position;
    Vector3 normal;
    Vector2 texture;
};

struct FaceVertex {
    int vertexIndex;
    int textureIndex;
    int normalIndex;
};

class Model {
   public:
    void SetVerticies(const std::vector<Vector3> &otherVerticies);
    void SetNormals(const std::vector<Vector2> &otherTexCoords);
    void SetTexCoords(const std::vector<Vector3> &otherNormals);

   private:
    std::vector<Vector3> m_verticies;
    std::vector<Vector3> m_normals;
    std::vector<Vector2> m_texCoords;
};
}  // namespace s21
