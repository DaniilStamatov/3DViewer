#include "OpenGLWindow.h"
OpenGLWindow::OpenGLWindow(QWidget *parent) : QOpenGLWidget(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);
  lastFrame = 0.0f;
  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &OpenGLWindow::updateAngle);
  timer->start(16);
   setFocusPolicy(Qt::StrongFocus); // Убедитесь, что виджет может получать фокус
    setFocus(); // Установите фокус на виджет
}

OpenGLWindow::~OpenGLWindow()
{

}

void OpenGLWindow::ChangeCurrentModel()
{
  m_currentModelIndex = (m_currentModelIndex + 1) % m_models.size();
  std::cout << m_currentModelIndex << std::endl;
  m_currentModel = m_models[m_currentModelIndex];
}

void OpenGLWindow::loadModel(const std::string &filename) {
  if (!QOpenGLContext::currentContext()) {
    std::cerr << "OpenGL context is not current!" << std::endl;
  }
  makeCurrent();
  m_loader.Clear();
  m_loader.LoadFromFile(filename);
  modelLoaded = false;
  functions = QOpenGLContext::currentContext()->extraFunctions();
  m_models.push_back(std::make_shared<ModelRenderer>(functions, m_loader));
  m_currentModelIndex = m_models.size() - 1;
  m_currentModel = m_models[m_currentModelIndex];
  modelLoaded = true; 
}

void OpenGLWindow::SetObjectPosition(float x, float y, float z) {
  m_currentModel.get()->SetObjectPosition(x, y, z);
}

void OpenGLWindow::SetObjectRotation(float x, float y, float z) {
  m_currentModel.get()->SetObjectRotation(x, y, z);
}

void OpenGLWindow::SetObjectScale(float x, float y, float z) {
  m_currentModel.get()->SetObjectScale(x, y, z);
}

void OpenGLWindow::SetLinesColor(float x, float y, float z) {
  m_currentModel.get()->SetLinesColor(x, y, z);
}

void OpenGLWindow::initializeGL() {
  timer.start();
  functions = QOpenGLContext::currentContext()->extraFunctions();
  functions->initializeOpenGLFunctions();
  this->makeCurrent();
  functions->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  loadModel("Meshes/Male.OBJ");
  loadModel("Meshes/Chairs.obj");
  m_currentModelIndex = m_models.size() - 1;
  m_currentModel = m_models[m_currentModelIndex];
  functions->glEnableVertexAttribArray(0);
  functions->glBindVertexArray(0);
}

void OpenGLWindow::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void OpenGLWindow::paintGL() {
  makeCurrent();
  float currentFrame = static_cast<float>(timer.elapsed()) / 1000.0f;
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
  functions = QOpenGLContext::currentContext()->extraFunctions();
  functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  s21::Matrix4x4 projection = s21::perspective(m_camera.GetZoom(), (float)1920 / (float)1080, 0.1f, 100.0f);
  s21::Matrix4x4 view = m_camera.GetViewMatrix();
  functions->glLineWidth(1.0f);
  for(size_t i = 0; i < m_models.size(); i++) {
     m_models[i]->Draw(projection, view);
  }
  GLenum error = glGetError();
  if(error != GL_NO_ERROR) {
      std::cout << "error here" << std::endl;
  }
  functions->glBindVertexArray(0);
}

void OpenGLWindow::keyPressEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_W) {
    m_camera.ProcessKeyboard(FORWARD, deltaTime);
    std::cout << "W" << std::endl;
  }
  if(event->key() == Qt::Key_S) {
    m_camera.ProcessKeyboard(BACKWARD, deltaTime);
  }
  if(event->key() == Qt::Key_D) {
    m_camera.ProcessKeyboard(RIGHT, deltaTime);
  }
  if(event->key() == Qt::Key_A) {
    m_camera.ProcessKeyboard(LEFT, deltaTime);
  }
}
