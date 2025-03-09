#pragma once
#include <iostream>
#include <sstream>

#include "model.h"
namespace s21 {
class Loader {
   public:
    Loader() = default;
    void LoadFromFile(const std::string &filename);
    void ParseFace(std::vector<Vertex>& verts, std::istringstream &stream);
    void VertexTriangluation(std::vector<unsigned int>& indices, const std::vector<Vertex>& Verts);
    std::vector<Vertex> GetVerticies() const;
    std::vector<Vector2> GetTexturesCoords() const;
    std::vector<Vector3> GetNormals() const;
    std::vector<FaceVertex> GetFaces() const;
    std::vector<unsigned int> GetIndices() const;
    void PrintLoadedInfo();
    void Clear();
    void RoundNormal(Vector3& normal);

   private:
    Model model;
    std::vector<Vector3> m_verticies;
    std::vector<Vertex> m_vertices;
    std::vector<Vector3> m_normals;
    std::vector<Vector2> m_texCoords;
    std::vector<FaceVertex> m_faceVerticies;
    std::vector<unsigned int> m_indices;
};
} //namespace s21