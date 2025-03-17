#pragma once
#include "Loader.h"
#include "shader.h"
#include "Texture.h"
enum class DrawMode { TRIANGLES, LINES };

class MeshRenderer{ 
    public:
    MeshRenderer(QOpenGLExtraFunctions *functions, const std::vector<s21::Vertex>& vertices, const std::vector<unsigned int>& indices, const s21::Material& material) {
        m_functions = functions;
        m_vertices = vertices;
        m_indices = indices;
        m_material = material;
        functions->glGenVertexArrays(1, &m_vao);
        functions->glGenBuffers(1, &m_vbo);
        functions->glGenBuffers(1, &m_eboLines);
        functions->glGenBuffers(1, &m_normalEBO);
        functions->glGenBuffers(1, &m_normalVBO);
        functions->glGenBuffers(1, &m_eboTriangles);
        SetUp();
    }
    void SetUp() {
        m_functions->glBindVertexArray(m_vao);
        m_functions->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        m_functions->glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(s21::Vertex),
                                  m_vertices.data(), GL_STATIC_DRAW);

        std::vector<unsigned int> indices;
        indices.reserve(m_indices.size());
        for (const auto& index : m_indices) {
            indices.emplace_back(index);
        }

        m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboTriangles);
        m_functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(),
                                  GL_STATIC_DRAW);

        std::vector<unsigned int> lineIndices;
        for (size_t i = 0; i < indices.size(); i += 3) {
            lineIndices.push_back(indices[i]);
            lineIndices.push_back(indices[i + 1]);
            lineIndices.push_back(indices[i + 1]);
            lineIndices.push_back(indices[i + 2]);
            lineIndices.push_back(indices[i + 2]);
            lineIndices.push_back(indices[i]);
        }
    
        m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboLines);
        m_functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, lineIndices.size() * sizeof(unsigned int), lineIndices.data(),
                                  GL_STATIC_DRAW);

        m_functions->glEnableVertexAttribArray(0);
        m_functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(s21::Vertex), (void *)0);
        m_functions->glEnableVertexAttribArray(1);
        m_functions->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(s21::Vertex), (void *)offsetof(s21::Vertex, normal));
        m_functions->glBindVertexArray(0);
    }

    void Draw(DrawMode mode, Shader& shader) {
        m_functions->glBindVertexArray(m_vao);
        SetShaderProperties(shader);
        if (mode == DrawMode::LINES) {
            m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboLines);
            m_functions->glDrawElements(GL_LINES, m_indices.size() * 3, GL_UNSIGNED_INT, 0);
        } else if (mode == DrawMode::TRIANGLES) {
            m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboTriangles);
            m_functions->glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
        }

        m_functions->glBindVertexArray(0);
    }
    
    void SetShaderProperties(Shader& shader) {
        shader.SetUniform3f("u_material.ambient", m_material.ambient);
        shader.SetUniform3f("u_material.diffuse", m_material.diffuse);
        shader.SetUniform3f("u_material.specular", m_material.specular);
        shader.SetUniform1f("u_material.Ns", m_material.Ns);
        shader.SetUniform1f("u_material.Ni", m_material.Ni);
        shader.SetUniform1f("u_material.d", m_material.d);
    }
    private:
        QOpenGLExtraFunctions *m_functions;
        std::vector<s21::Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        unsigned int m_vao;
        unsigned int m_vbo;
        unsigned int m_normalVBO;
        unsigned int m_normalEBO;
        unsigned int m_eboLines;
        unsigned int m_eboTriangles;
        s21::Material m_material;
        DrawMode m_drawMode = DrawMode::TRIANGLES;
};