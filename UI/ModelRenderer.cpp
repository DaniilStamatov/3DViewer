#include "ModelRenderer.h"

ModelRenderer::ModelRenderer(QOpenGLExtraFunctions *functions, const s21::Loader &loader)
    : m_shader(functions, "shaders/basic.shader") {
    m_transform = s21::Matrix4x4();
    m_linesColor = s21::Vector3(1.0, 0.5, 0.3);
    m_loader = loader;
    m_functions = functions;
    functions->glGenVertexArrays(1, &m_vao);
    functions->glGenBuffers(1, &m_vbo);
    functions->glGenBuffers(1, &m_eboLines);
    functions->glGenBuffers(1, &m_eboTriangles);
    SetUp();
}

ModelRenderer::~ModelRenderer() {}

void ModelRenderer::SetUp() {
    m_functions->glBindVertexArray(m_vao);
    m_functions->glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    m_functions->glBufferData(GL_ARRAY_BUFFER, m_loader.GetVerticies().size() * sizeof(s21::Vector3),
                              m_loader.GetVerticies().data(), GL_STATIC_DRAW);

    std::vector<unsigned int> indices;
    indices.reserve(m_loader.GetFaces().size());
    const std::vector<s21::FaceVertex> &m_faceVerticies = m_loader.GetFaces();
    for (const auto& faceVertice : m_faceVerticies) {
        indices.emplace_back(faceVertice.vertexIndex);
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
    m_functions->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(s21::Vector3), (void *)0);
    m_functions->glBindVertexArray(0);
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
    m_linesColor = s21::Vector3(
        std::clamp(x / 255.0f, 0.0f, 1.0f),
        std::clamp(y / 255.0f, 0.0f, 1.0f),
        std::clamp(z / 255.0f, 0.0f, 1.0f)
    );
}

// s21::Vector3 ModelRenderer::GetPosition() const { return s21::Vector3(m_positionMatrix[3][0], m_positionMatrix[3][1], m_positionMatrix[3][2]); }

// s21::Vector3 ModelRenderer::GetRotation() const { 
  
// }

// s21::Vector3 ModelRenderer::GetScale() const { return s21::Vector3(m_scaleMatrix[3][0], m_scaleMatrix[3][1], m_scaleMatrix[3][2]); }

void ModelRenderer::ParseTransform(s21::Vector3& position, s21::Vector3 &scale, s21::Vector3 &rotation) {
  
  s21::Matrix4x4 localMat(m_transform);
  position = s21::Vector3(localMat[3][0], localMat[3][1], localMat[3][2]);

    localMat[3] = {0, 0, 0, localMat[3].w};
    s21::Vector3 Row[3];
  		for (int i = 0; i < 3; ++i)
  			for (int j = 0; j < 3; ++j)
  				Row[i][j] = localMat[i][j];

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
		}
		else {
			rotation.x = atan2(-Row[2][0], Row[1][1]);
			rotation.z = 0;
		}
}

void ModelRenderer::SwitchDrawMode() {
    m_drawMode = m_drawMode == DrawMode::LINES ? DrawMode::TRIANGLES : DrawMode::LINES;
}

void ModelRenderer::Draw(const s21::Matrix4x4 &projection, const s21::Matrix4x4 &view) {
    m_functions->glBindVertexArray(m_vao);
    m_shader.Bind();
    m_shader.SetUniformMat4f("u_projection", projection);
    m_shader.SetUniformMat4f("u_view", view);
    m_shader.SetUniform3f("u_color", m_linesColor);
    m_transform = m_positionMatrix * m_scaleMatrix * m_rotationMatrix;

    m_shader.SetUniformMat4f("u_model", m_transform);

    if (m_drawMode == DrawMode::LINES) {
        m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboLines);
        m_functions->glDrawElements(GL_LINES, m_loader.GetFaces().size() * 3, GL_UNSIGNED_INT, 0);
    } else if (m_drawMode == DrawMode::TRIANGLES) {
        m_functions->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboTriangles);
        m_functions->glDrawElements(GL_TRIANGLES, m_loader.GetFaces().size(), GL_UNSIGNED_INT, 0);
    }

    m_functions->glBindVertexArray(0);
    m_shader.Unbind();
}