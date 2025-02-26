#include "../loader.h"
#include "../shader/shader.h"

#include <QOpenGLExtraFunctions>
#include <QOpenGLWidget>
class ModelRenderer : QOpenGLExtraFunctions {
public:
    ModelRenderer(const ModelRenderer&) = delete;
    ModelRenderer& operator=(const ModelRenderer&) = delete;

    // Разрешите перемещение
    ModelRenderer(ModelRenderer&&) = default;
    ModelRenderer& operator=(ModelRenderer&&) = default;
    explicit ModelRenderer(QOpenGLExtraFunctions* functions, const s21::Loader& loader);
    void SetObjectPosition(float x, float y, float z);
    void SetObjectRotation(float x, float y, float z);
    void SetObjectScale(float x, float y, float z);
    void SetLinesColor(float x, float y, float z);
    ~ModelRenderer();
    void Draw(const s21::Matrix4x4& projection, const s21::Matrix4x4& view);
private:
    QOpenGLExtraFunctions* m_functions;
    s21::Loader m_loader;
    s21::Matrix4x4 m_positionMatrix;
    s21::Matrix4x4 m_rotationMatrix;
    s21::Matrix4x4 m_scaleMatrix;
    s21::Matrix4x4 m_transform;
    s21::Vector3 m_linesColor;
    Shader m_shader;

    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ebo;
};