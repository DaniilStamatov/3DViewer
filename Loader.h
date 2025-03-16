#pragma once
#include <iostream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <unordered_map>

#include "model.h"

namespace s21 {
struct Mesh {
    Mesh () {
        
    }
    Mesh(std::vector<s21::Vertex>& _Vertices, std::vector<unsigned int>& _Indices)
    {
    	m_vertices = _Vertices;
    	m_indices = _Indices;
    }
    std::string m_meshName;
    std::vector<s21::Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    s21::Material m_material;
};
class Loader {
   public:
    Loader() = default;
    void LoadFromFile(const std::string& filename);
    void ParseMaterial(const std::string& filename, std::istringstream& iss);
    void SetMeshMaterial(std::vector<unsigned int>& tempIndices, std::vector<s21::Vertex>& tempVertices,
                         std::string& tempMeshname, std::istringstream& iss, Material& tempMaterial);
    void ParseFace(std::vector<Vertex>& verts, std::istringstream &stream);
    void VertexTriangluation(std::vector<unsigned int>& indices, const std::vector<Vertex>& Verts);
    std::vector<Mesh> GetMeshes() const;
    std::vector<Vector2> GetTexturesCoords() const;
    std::vector<Vector3> GetNormals() const;
    std::vector<FaceVertex> GetFaces() const;
    std::vector<unsigned int> GetIndices() const;
    void PrintLoadedInfo();
    void Clear();
    void RoundNormal(Vector3& normal);
    bool LoadMaterials(const std::string& path);

   private:
    Model model;
    std::unordered_map<std::string, Material> m_materials;
    std::vector<Mesh> m_meshes;
    std::vector<Vector3> m_verticies;
    std::vector<Vertex> m_vertices;
    std::vector<Vector3> m_normals;
    std::vector<Vector2> m_texCoords;
    std::vector<FaceVertex> m_faceVerticies;
    std::vector<unsigned int> m_indices;
};
} //namespace s21