#pragma once
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLBuffer>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWidget>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <chrono>

#include "../Camera/Camera.h"
#include "../Controller/ModelController.h"
#include "../Loader.h"
#include "../shader/shader.h"
#include "../transformations/transformation.h"
#include "../vendor/glm/glm.hpp"
#include "../vendor/glm/gtc/matrix_transform.hpp"
#include "../vendor/glm/gtc/type_ptr.hpp"
#include "ModelRenderer.h"

class OpenGLWindow : public QOpenGLWidget {
    Q_OBJECT
   public:
    OpenGLWindow(QWidget *parent = nullptr);
    ~OpenGLWindow();
    void loadModel(const std::string &filename);
    void SetCurrentObjectPosition(float x, float y, float z);
    void SetCurrentObjectRotation(float x, float y, float z);
    void SetCurrentObjectScale(float x, float y, float z);
    s21::Vector3 GetCurrentPosition() const;
    s21::Vector3 GetCurrentRotation() const;
    s21::Vector3 GetCurrentScale() const;
    void SetCurrentLinesColor(float x, float y, float z);
    void ParseTransform(s21::Vector3& position, s21::Vector3 &scale, s21::Vector3 &rotation);
    void SwitchDrawMode();
    void ProcessInput();
   private slots:
    void onTimeout() { update(); }
   signals:
    void changeCurrentModel();
   protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
   
   
   private:
    float deltaTime = 0.0f;
    Camera m_camera;
    bool modelLoaded = false;
    float angle;
    QOpenGLExtraFunctions *functions;
    s21::Loader m_loader;
    std::vector<ModelRenderer> m_models;
    std::unique_ptr<ModelController> m_modelController;
    QElapsedTimer m_timer;
    std::chrono::high_resolution_clock::time_point lastFrame;
    unsigned int VBO;
    unsigned int lightCubeVAO;
    Shader* lightShader;
    bool isDragging = false;
    QPoint lastMousePos;
     float lastX = 0.0f; // Последняя позиция X
    float lastY = 0.0f; 
     bool firstMouse = true;
};