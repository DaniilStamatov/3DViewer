#include "OpenGLWindow.h"
OpenGLWindow::OpenGLWindow(QWidget* parent) : QOpenGLWidget(parent){
    QVBoxLayout* layout = new QVBoxLayout(this);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &OpenGLWindow::updateAngle);
    timer->start(16);
    transform = s21::Matrix4x4();
}

void OpenGLWindow::loadModel(const std::string& filename) {
    m_loader.LoadFromFile(filename);
    update();
}

void OpenGLWindow::SetObjectPosition(float x, float y, float z)
{
    positionMatrix = s21::Translate(s21::Matrix4x4(), s21::Vector3(x, y, z));
}

void OpenGLWindow::SetObjectRotation(float x, float y, float z)
{
    rotationMatrix = s21::Matrix4x4();
    rotationMatrix = s21::Rotate(rotationMatrix, z, s21::Vector3(0, 0, 1));
    rotationMatrix = s21::Rotate(rotationMatrix, y, s21::Vector3(0, 1, 0));
    rotationMatrix = s21::Rotate(rotationMatrix, x, s21::Vector3(1, 0, 0));
}

void OpenGLWindow::SetObjectScale(float x, float y, float z)
{
    scaleMatrix = s21::Matrix4x4();
    scaleMatrix = s21::Scale(scaleMatrix, s21::Vector3(x, y, z));
}

void OpenGLWindow::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    QOpenGLExtraFunctions* functions = new QOpenGLExtraFunctions(this->context());
    m_shader = new Shader(functions, "shaders/basic.shader");
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_loader.GetVerticies().size() * sizeof(s21::Vector3), m_loader.GetVerticies().data(), GL_STATIC_DRAW);
    std::vector<unsigned int> indices;
    for (const auto& face : m_loader.GetFaces()) {
        indices.push_back(face.vertexIndex); 
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(s21::Vector3), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void OpenGLWindow::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
}

void OpenGLWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(m_vao);
    glLineWidth(3.0f);
    transform =scaleMatrix * rotationMatrix *  positionMatrix;
    m_shader->Bind();
    m_shader->SetUniformMat4f("mvp", transform);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cerr << "OpenGL error occurred: " << error << std::endl;
    }
    glDrawElements(GL_LINES, m_loader.GetFaces().size() * 3, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);
}