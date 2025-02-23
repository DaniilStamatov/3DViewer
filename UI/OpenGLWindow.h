#pragma once
#include "../loader.h"
#include "../shader/shader.h"
#include "../transformations/transformation.h"
#include "ModelRenderer.h"
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

class OpenGLWindow : public QOpenGLWidget {
public:
  OpenGLWindow(QWidget *parent = nullptr);
  ~OpenGLWindow();
  void ChangeCurrentModel();
  void loadModel(const std::string &filename);
  void SetObjectPosition(float x, float y, float z);
  void SetObjectRotation(float x, float y, float z);
  void SetObjectScale(float x, float y, float z);
  void SetLinesColor(float x, float y, float z);
private slots:
  void updateAngle() {
    angle += 0.0005f;
    if (angle > 360) {
      angle -= 360;
    }
    update();
  }

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;

private:
  bool modelLoaded = false;
  float angle;
  QOpenGLExtraFunctions *functions;
  GLuint m_vao, m_vbo, m_ebo, vboTexCoords, vboNormals;
  s21::Loader m_loader;
  size_t m_currentModelIndex;
  std::shared_ptr<ModelRenderer> m_currentModel;
  std::vector<std::shared_ptr<ModelRenderer>> m_models;
};