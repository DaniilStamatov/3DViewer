#include "ModelRenderer.h"

ModelRenderer::ModelRenderer(QOpenGLExtraFunctions* functions, const s21::Loader &loader) : m_shader(functions, "shaders/basic.shader")
{

    m_transform = s21::Matrix4x4();
    m_linesColor = s21::Vector3(1.0, 0.5, 0.3);
    m_loader = loader;
    m_functions = functions;
    functions->glGenVertexArrays(1, &m_vao);
    functions->glGenBuffers(1, &m_vbo);
    functions->glGenBuffers(1, &m_ebo);
    functions->glBindVertexArray(m_vao);
    functions->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    functions->glBufferData(GL_ARRAY_BUFFER,
                 m_loader.GetVerticies().size() * sizeof(s21::Vector3),
                 m_loader.GetVerticies().data(), GL_STATIC_DRAW);
    std::vector<unsigned int> indices;
    for (const auto &face : m_loader.GetFaces()) {
      indices.push_back(face.vertexIndex);
    }
    functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    functions->glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
    &indices[0], GL_STATIC_DRAW);
    functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(s21::Vector3),
          (void *)0);
    functions->glEnableVertexAttribArray(0);
    functions->glBindVertexArray(0);
}

ModelRenderer::~ModelRenderer() {
  m_functions->glDeleteVertexArrays(1, &m_vao);
  m_functions->glDeleteBuffers(1, &m_vbo);
  m_functions->glDeleteBuffers(1, &m_ebo);
}

void ModelRenderer::Draw(const s21::Matrix4x4& projection, const s21::Matrix4x4& view)
{
    m_functions->glBindVertexArray(m_vao);
    m_transform = m_scaleMatrix * m_rotationMatrix * m_positionMatrix;
    m_shader.Bind();
    m_shader.SetUniformMat4f("u_model", m_transform);
    m_shader.SetUniformMat4f("u_projection", projection);
    m_shader.SetUniformMat4f("u_view", view);
    m_shader.SetUniform3f("u_color", m_linesColor);
    m_functions->glDrawElements(GL_LINES, m_loader.GetFaces().size(), GL_UNSIGNED_INT, nullptr);
    m_functions->glBindVertexArray(0);
    m_shader.Unbind();
}

void ModelRenderer::SetObjectPosition(float x, float y, float z) {
  m_positionMatrix = s21::Translate(s21::Matrix4x4(), s21::Vector3(x, y, z));
}

void ModelRenderer::SetObjectRotation(float x, float y, float z) {
  m_rotationMatrix = s21::Matrix4x4();
  m_rotationMatrix =
      s21::Rotate(m_rotationMatrix, z * (M_PI / 180.0), s21::Vector3(0, 0, 1));
  m_rotationMatrix =
      s21::Rotate(m_rotationMatrix, y * (M_PI / 180.0), s21::Vector3(0, 1, 0));
  m_rotationMatrix =
      s21::Rotate(m_rotationMatrix, x * (M_PI / 180.0), s21::Vector3(1, 0, 0));
}

void ModelRenderer::SetObjectScale(float x, float y, float z) {
  m_scaleMatrix = s21::Matrix4x4();
  m_scaleMatrix = s21::Scale(m_scaleMatrix, s21::Vector3(x, y, z));
}

void ModelRenderer::SetLinesColor(float x, float y, float z) {
  float normalizedRed = x / 255.0f;
  float normalizedGreen = y / 255.0f;
  float normalizedBlue = z / 255.0f;

  s21::Vector3 temp(normalizedRed, normalizedGreen, normalizedBlue);
  m_linesColor = temp;
}