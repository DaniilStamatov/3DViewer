#pragma once
#include "model.h"
#include <iostream>
#include <sstream>
namespace s21 {
class Loader {
public:
  Loader() = default;
  void LoadFromFile(const std::string &filename);
  void ParseFace(std::istringstream &stream);
  std::vector<Vector3> GetVerticies() const;
  std::vector<Vector2> GetTexturesCoords() const;
  std::vector<Vector3> GetNormals() const;
  std::vector<FaceVertex> GetFaces() const;
  void PrintLoadedInfo();

private:
  Model model;
  std::vector<Vector3> m_verticies;
  std::vector<Vector2> m_texCoords;
  std::vector<Vector3> m_normals;
  std::vector<FaceVertex> m_faceVerticies;
};
} // namespace s21