#include "OpenGLWindow.h"
OpenGLWindow::OpenGLWindow(QWidget *parent) : QOpenGLWidget(parent) {
  QVBoxLayout *layout = new QVBoxLayout(this);

  QTimer *timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &OpenGLWindow::updateAngle);
  timer->start(16);
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
  functions = QOpenGLContext::currentContext()->extraFunctions();
  functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(modelLoaded) {
    functions->glLineWidth(1.0f);
    for(size_t i = 0; i < m_models.size(); i++) {
       m_models[i]->Draw();
    }
    GLenum error = glGetError();
    if(error != GL_NO_ERROR) {
        std::cout << "error here" << std::endl;
    }
  }
  functions->glBindVertexArray(0);
}