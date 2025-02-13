#pragma once

#include <fstream>
#include <vector>

namespace s21 {
    struct Vector2 {
        Vector2() : x(0.0f), y(0.0f) {}
        Vector2(int x_, int y_) : x(x_), y(y_) {}
        Vector2(const Vector2& other) : x(other.x), y(other.y) {}
        float x, y;
    };

   

    struct Vector3 {
        Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
        Vector3(int x_, int y_, int z_) : x(x_), y(y_), z(z_) {}
        Vector3(const Vector3& other) : x(other.x), y(other.y), z(other.z) {}
        float x, y, z;
    };

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
        
        void SetVerticies(const std::vector<Vector3>& otherVerticies);
        void SetNormals(const std::vector<Vector2>& otherTexCoords);
        void SetTexCoords(const std::vector<Vector3>& otherNormals);
    private:
        std::vector<Vector3> m_verticies;
        std::vector<Vector3> m_normals;
        std::vector<Vector2> m_texCoords;
    };
}
