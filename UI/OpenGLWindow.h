#pragma once
#include "../loader.h"
#include "../shader/shader.h"
#include "../transformations/transformation.h"
#include "../Camera/camera.h"
#include "ModelRenderer.h"
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <QKeyEvent>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QElapsedTimer>
#include <chrono>

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
  void ProcessInput();
private slots:
  void onTimeout() {
    update();
  }

protected:
  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void keyPressEvent(QKeyEvent *event);

private:
  float deltaTime = 0.0f; 
  Camera m_camera;
  bool modelLoaded = false;
  float angle;
  QOpenGLExtraFunctions *functions;
  GLuint m_vao, m_vbo, m_ebo, vboTexCoords, vboNormals;
  s21::Loader m_loader;
  size_t m_currentModelIndex;
  std::shared_ptr<ModelRenderer> m_currentModel;
  std::vector<std::shared_ptr<ModelRenderer>> m_models;

  QElapsedTimer m_timer; // Таймер для измерения времени
  std::chrono::high_resolution_clock::time_point lastFrame;     // Время последнего кадра
};