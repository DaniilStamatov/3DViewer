#pragma once
#include "MeshRenderer.h"

#include <QOpenGLWidget>

class ModelRenderer : QOpenGLExtraFunctions {
   public:
    ~ModelRenderer();
    ModelRenderer(ModelRenderer&&) = default;
    ModelRenderer& operator=(ModelRenderer&&) = default;
    explicit ModelRenderer(QOpenGLExtraFunctions* functions, const s21::Loader& loader);

    void SetUp();
    void SetObjectPosition(float x, float y, float z);
    void SetObjectRotation(float x, float y, float z);
    void SetObjectScale(float x, float y, float z);
    void SetLinesColor(float x, float y, float z);
    s21::Vector3 GetPosition() const;
    s21::Vector3 GetRotation() const;
    s21::Vector3 GetScale() const;
    void ParseTransform(s21::Vector3& position, s21::Vector3& scale, s21::Vector3& rotation);
    void SwitchDrawMode();
    void Draw(const glm::mat4& projection, const s21::Matrix4x4& view, const s21::Vector3& cameraPos);
    void ProcessMesh(s21::Mesh* mesh);
   private:
    QOpenGLExtraFunctions* m_functions;
    s21::Loader m_loader;
    s21::Matrix4x4 m_positionMatrix;
    s21::Matrix4x4 m_rotationMatrix;
    s21::Matrix4x4 m_scaleMatrix;
    s21::Matrix4x4 m_transform;
    s21::Vector3 m_linesColor;
    std::vector<MeshRenderer> m_meshes;
    Texture m_texture;
    Shader m_shader;

    DrawMode m_drawMode = DrawMode::TRIANGLES;
    
};