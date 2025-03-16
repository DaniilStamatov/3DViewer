#include "ModelRenderer.h"

ModelRenderer::ModelRenderer(QOpenGLExtraFunctions *functions, const s21::Loader &loader)
    : m_shader(functions, "shaders/basic.shader"), m_texture(functions, "res/images/awesomeface.png") {
    m_transform = s21::Matrix4x4();
    m_linesColor = s21::Vector3(1.0, 0.5, 0.3);
    m_loader = loader;
    m_functions = functions;
    SetUp();
}

ModelRenderer::~ModelRenderer() {}

void ModelRenderer::SetUp() {
    m_meshes.reserve(m_loader.GetMeshes().size());
    for (auto mesh : m_loader.GetMeshes()) {
        m_meshes.emplace_back(m_functions, mesh.m_vertices, mesh.m_indices, mesh.m_material);
    }
}

void ModelRenderer::SetObjectPosition(float x, float y, float z) {
    m_positionMatrix = s21::Translate(s21::Matrix4x4(), s21::Vector3(x, y, z));
}

void ModelRenderer::SetObjectRotation(float x, float y, float z) {
    m_rotationMatrix = s21::Rotate(s21::Matrix4x4(), z * (M_PI / 180.0), s21::Vector3(0, 0, 1));
    m_rotationMatrix = s21::Rotate(m_rotationMatrix, y * (M_PI / 180.0), s21::Vector3(0, 1, 0));
    m_rotationMatrix = s21::Rotate(m_rotationMatrix, x * (M_PI / 180.0), s21::Vector3(1, 0, 0));
}

void ModelRenderer::SetObjectScale(float x, float y, float z) {
    m_scaleMatrix = s21::Matrix4x4();
    m_scaleMatrix = s21::Scale(m_scaleMatrix, s21::Vector3(x, y, z));
}

void ModelRenderer::SetLinesColor(float x, float y, float z) {
    m_linesColor = s21::Vector3(std::clamp(x / 255.0f, 0.0f, 1.0f), std::clamp(y / 255.0f, 0.0f, 1.0f),
                                std::clamp(z / 255.0f, 0.0f, 1.0f));
}

// s21::Vector3 ModelRenderer::GetPosition() const { return s21::Vector3(m_positionMatrix[3][0], m_positionMatrix[3][1],
// m_positionMatrix[3][2]); }

// s21::Vector3 ModelRenderer::GetRotation() const {

// }

// s21::Vector3 ModelRenderer::GetScale() const { return s21::Vector3(m_scaleMatrix[3][0], m_scaleMatrix[3][1],
// m_scaleMatrix[3][2]); }

void ModelRenderer::ParseTransform(s21::Vector3 &position, s21::Vector3 &scale, s21::Vector3 &rotation) {
    s21::Matrix4x4 localMat(m_transform);
    position = s21::Vector3(localMat[3][0], localMat[3][1], localMat[3][2]);

    localMat[3] = {0, 0, 0, localMat[3].w};
    s21::Vector3 Row[3];
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j) Row[i][j] = localMat[i][j];

    scale.x = Row[0].length();
    Row[0] = s21::scale(Row[0], 1);
    scale.y = Row[1].length();
    Row[1] = s21::scale(Row[1], 1);
    scale.y = Row[2].length();
    Row[2] = s21::scale(Row[2], 1);
    rotation.y = asin(-Row[0][2]);
    if (cos(rotation.y) != 0) {
        rotation.x = atan2(Row[1][2], Row[2][2]);
        rotation.z = atan2(Row[0][1], Row[0][0]);
    } else {
        rotation.x = atan2(-Row[2][0], Row[1][1]);
        rotation.z = 0;
    }
}

void ModelRenderer::SwitchDrawMode() {
    m_drawMode = m_drawMode == DrawMode::LINES ? DrawMode::TRIANGLES : DrawMode::LINES;
}

void ModelRenderer::Draw(const glm::mat4 &projection, const s21::Matrix4x4 &view, const s21::Vector3 &cameraPos) {
    m_functions->glEnable(GL_DEPTH_TEST);

    m_shader.Bind();
    m_shader.SetUniformMat4f("u_projection", projection);
    m_shader.SetUniformMat4f("u_view", view);
    m_shader.SetUniform3f("u_viewPosition", cameraPos);
    m_shader.SetUniform3f("u_color", m_linesColor);
    m_shader.SetUniform3f("u_lightPosition", s21::Vector3(2.0, 2.0, 2.0));
    m_shader.SetUniform3f("u_lightColor", s21::Vector3(1.0, 1.0, 1.0));
    m_transform = m_positionMatrix * m_rotationMatrix * m_scaleMatrix;
    m_texture.Bind();
    m_shader.SetUniform1i("u_texDiffuse", 0);
    m_shader.SetUniformMat4f("u_model", m_transform);
    for (auto mesh : m_meshes) {
        mesh.Draw(m_drawMode, m_shader);
    }
    GLenum err;
    while ((err = m_functions->glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGL error: " << err << std::endl;
    }

    m_functions->glBindVertexArray(0);
    m_shader.Unbind();
}