#pragma once
#include "../loader.h"
#include "../shader/shader.h"
#include "../transformations/transformation.h"
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

class OpenGLWindow : public QOpenGLWidget, public QOpenGLExtraFunctions {
public:
  OpenGLWindow(QWidget *parent = nullptr);
  void loadModel(const std::string &filename);
  void SetObjectPosition(float x, float y, float z);
  void SetObjectRotation(float x, float y, float z);
  void SetObjectScale(float x, float y, float z);
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
  float angle;
  s21::Matrix4x4 positionMatrix;
  s21::Matrix4x4 rotationMatrix;
  s21::Matrix4x4 scaleMatrix;
  s21::Matrix4x4 transform;

  GLuint m_vao, m_vbo, m_ebo, vboTexCoords, vboNormals;
  s21::Loader m_loader;
  Shader *m_shader;
};