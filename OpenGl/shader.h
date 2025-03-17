#pragma once
#include "mat4.h"
#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

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
    Shader(const Shader &other)
        : m_functions(other.m_functions),
          m_rendererID(other.m_rendererID),
          m_filepath(other.m_filepath),
          m_UniformLocationCashe(other.m_UniformLocationCashe)
    {
        ShaderProgramSource source = ParseShader(m_filepath);
        m_rendererID = CreateShader(source.VertexSource, source.FragmentSource);
    }

    Shader &operator=(const Shader &other) {
        if (this != &other) {
            m_functions = other.m_functions;
            m_rendererID = other.m_rendererID;
            m_filepath = other.m_filepath;
            m_UniformLocationCashe = other.m_UniformLocationCashe;
            ShaderProgramSource source = ParseShader(m_filepath);
            m_rendererID = CreateShader(source.VertexSource, source.FragmentSource);
        }
        return *this;
    }

    Shader(QOpenGLExtraFunctions *functions, const std::string &filename);
    void Bind();
    void Unbind();
    void SetUniformMat4f(const std::string &name, const s21::Matrix4x4 &mat);
    void SetUniformMat4f(const std::string &name, const glm::mat4 &mat);
    void SetUniform3f(const std::string &name, const s21::Vector3 &vector);
    void SetUniform1i(const std::string &name, int num);
    void SetUniform1f(const std::string &name, float num);

    uint GetRendererID() const;
    ~Shader();

   private:
    QOpenGLExtraFunctions *m_functions;
    ShaderProgramSource ParseShader(const std::string &filename);
    uint CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
    uint CompileShader(const std::string &source, uint type);
    int GetUniformLocation(const std::string &name);
    uint m_rendererID;
    std::unordered_map<std::string, int> m_UniformLocationCashe;
    std::string m_filepath;
};