#include "OpenGLWindow.h"
OpenGLWindow::OpenGLWindow(QWidget *parent) : QOpenGLWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    lastFrame = std::chrono::high_resolution_clock::now();

    m_timer.start();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &OpenGLWindow::onTimeout);
    timer->start(16);
    setFocusPolicy(Qt::StrongFocus);
    setFocus();
    m_modelController = std::make_unique<ModelController>(m_models);
}

OpenGLWindow::~OpenGLWindow() {}

void OpenGLWindow::loadModel(const std::string &filename) {
    makeCurrent();
    m_loader.Clear();
    m_loader.LoadFromFile(filename);
    functions = QOpenGLContext::currentContext()->extraFunctions();
    m_models.emplace_back(functions, m_loader);
}

void OpenGLWindow::SetCurrentObjectPosition(float x, float y, float z) { m_modelController.get()->SetObjectPosition(x, y, z); }
void OpenGLWindow::SetCurrentObjectRotation(float x, float y, float z) { m_modelController.get()->SetObjectRotation(x, y, z); }
void OpenGLWindow::SetCurrentObjectScale(float x, float y, float z) { m_modelController.get()->SetObjectScale(x, y, z); }

// s21::Vector3 OpenGLWindow::GetCurrentPosition() const {
//    return m_modelController.get()->GetCurrentPosition();
// }

// s21::Vector3 OpenGLWindow::GetCurrentRotation() const {
//    return m_modelController.get()->GetCurrentRotation();
// }

// s21::Vector3 OpenGLWindow::GetCurrentScale() const {
//    return m_modelController.get()->GetCurrentScale();
// }

void OpenGLWindow::SetCurrentLinesColor(float x, float y, float z) { m_modelController.get()->SetLinesColor(x, y, z); }
void OpenGLWindow::ParseTransform(s21::Vector3 &position, s21::Vector3 &scale, s21::Vector3 &rotation) {
    return m_modelController.get()->ParseTransform(position, scale, rotation);
}
void OpenGLWindow::SwitchDrawMode() { m_modelController.get()->SwitchDrawMode(); }

void OpenGLWindow::initializeGL() {
    functions = QOpenGLContext::currentContext()->extraFunctions();
    functions->initializeOpenGLFunctions();
    this->makeCurrent();
    functions->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    functions->glEnableVertexAttribArray(0);
    functions->glBindVertexArray(0);
}

void OpenGLWindow::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void OpenGLWindow::paintGL() {
    functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    functions->glEnable(GL_DEPTH_TEST);
    auto currentFrameTime = std::chrono::high_resolution_clock::now();
    deltaTime = std::chrono::duration<float>(currentFrameTime - lastFrame).count();
    lastFrame = currentFrameTime;
    std::cout << "deltatime " << deltaTime << std::endl;

    s21::Matrix4x4 projection = s21::perspective(m_camera.GetZoom(), (float)1920 / (float)1080, 0.1f, 100.0f);
    s21::Matrix4x4 view = m_camera.GetViewMatrix();
    functions->glLineWidth(1.0f);
    for (size_t i = 0; i < m_models.size(); i++) {
        m_models[i].Draw(projection, view);
    }
    functions->glBindVertexArray(0);
}

void OpenGLWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Up) {
        m_modelController.get()->ChangeCurrentModel();
        emit changeCurrentModel();
    }
    if (event->key() == Qt::Key_W) {
        m_camera.ProcessKeyboard(FORWARD, deltaTime);
        std::cout << "W" << std::endl;
    }
    if (event->key() == Qt::Key_S) {
        m_camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (event->key() == Qt::Key_D) {
        m_camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (event->key() == Qt::Key_A) {
        m_camera.ProcessKeyboard(LEFT, deltaTime);
    }
}

 void OpenGLWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        lastMousePos = event->pos();
        isDragging = true;
    }
}

void OpenGLWindow::mouseMoveEvent(QMouseEvent *event) {
   if (isDragging) {
        float xpos = static_cast<float>(event->x());
        float ypos = static_cast<float>(event->y());

        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;

        lastX = xpos;
        lastY = ypos;

        m_camera.ProcessMouseMovement(xoffset, yoffset);
    }
}

void OpenGLWindow::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        firstMouse = true;
    }
}