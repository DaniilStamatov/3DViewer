#pragma once
#include "../transformations/mat4.h"
#include <QOpenGLExtraFunctions>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
struct ShaderProgramSource {
  std::string VertexSource;
  std::string FragmentSource;
};
class Shader {
public:
  Shader() = default;
  Shader(QOpenGLExtraFunctions *functions, const std::string &filename);
  void Bind();
  void Unbind();
  void SetUniformMat4f(const std::string &name, const s21::Matrix4x4 &mat);
  void SetUniform1f(const std::string &name, float val);
  uint GetRendererID() const;
  ~Shader();

private:
  QOpenGLExtraFunctions *m_functions;
  ShaderProgramSource ParseShader(const std::string &filename);
  uint CreateShader(const std::string &vertexShader,
                    const std::string &fragmentShader);
  uint CompileShader(const std::string &source, uint type);
  int GetUniformLocation(const std::string &name);
  uint m_rendererID;
  std::unordered_map<std::string, int> m_UniformLocationCashe;
  std::string m_filepath;
};