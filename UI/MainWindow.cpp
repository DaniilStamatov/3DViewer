#include "MainWindow.h"
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QHBoxLayout *layout = new QHBoxLayout(centralWidget);

    m_glWidget = new OpenGLWindow(this);
    layout->addWidget(m_glWidget);

    QWidget *buttonPanel = CreateButtonPanel();
    layout->addWidget(buttonPanel);
}

QWidget* MainWindow::CreateButtonPanel() {
    QWidget *buttonPanel = new QWidget(this);
    QVBoxLayout* settingsLayout = new QVBoxLayout(buttonPanel);

    settingsLayout->addWidget(new QLabel("Transform", this));
    settingsLayout->addLayout(CreateTransformLayout());
    settingsLayout->addLayout(CreateRotationLayout());
    settingsLayout->addLayout(CreateScaleLayout());
    QSpacerItem* spacer = new QSpacerItem(20, 200, QSizePolicy::Minimum, QSizePolicy::Expanding);
    settingsLayout->addItem(spacer);

    buttonPanel->setMaximumWidth(300);
    return buttonPanel;
}

QHBoxLayout* MainWindow::CreateTransformLayout() {
    QHBoxLayout *transformLayout = new QHBoxLayout();
    transformLayout->addWidget(new QLabel("Position", this));

    xCoordInput = new QLineEdit(this);
    xCoordInput->setPlaceholderText("X");
    xCoordInput->setMaximumWidth(100);
    transformLayout->addWidget(xCoordInput);

    yCoordInput = new QLineEdit(this);
    yCoordInput->setPlaceholderText("Y");
    yCoordInput->setMaximumWidth(100);
    transformLayout->addWidget(yCoordInput);

    zCoordInput = new QLineEdit(this);
    zCoordInput->setPlaceholderText("Z");
    zCoordInput->setMaximumWidth(100);
    transformLayout->addWidget(zCoordInput);
    connect(xCoordInput, &QLineEdit::returnPressed, this, &MainWindow::onCoordinateEntered);
    connect(yCoordInput, &QLineEdit::returnPressed, this, &MainWindow::onCoordinateEntered);
    connect(zCoordInput, &QLineEdit::returnPressed, this, &MainWindow::onCoordinateEntered);

    return transformLayout;
}

QHBoxLayout* MainWindow::CreateRotationLayout() {
    QHBoxLayout* rotationLayout = new QHBoxLayout();
    rotationLayout->addWidget(new QLabel("Rotation", this));

    xRotationInput = new QLineEdit(this);
    xRotationInput->setPlaceholderText("X");
    xRotationInput->setMaximumWidth(100);
    rotationLayout->addWidget(xRotationInput);

    yRotationInput = new QLineEdit(this);
    yRotationInput->setPlaceholderText("Y");
    yRotationInput->setMaximumWidth(100);
    rotationLayout->addWidget(yRotationInput);

    zRotationInput = new QLineEdit(this);
    zRotationInput->setPlaceholderText("Z");
    zRotationInput->setMaximumWidth(100);
    rotationLayout->addWidget(zRotationInput);
    connect(xRotationInput, &QLineEdit::returnPressed, this, &MainWindow::onRotationEntered);
    connect(yRotationInput, &QLineEdit::returnPressed, this, &MainWindow::onRotationEntered);
    connect(zRotationInput, &QLineEdit::returnPressed, this, &MainWindow::onRotationEntered);
    return rotationLayout;
}

QHBoxLayout* MainWindow::CreateScaleLayout() {
    QHBoxLayout* scaleLayout = new QHBoxLayout();
    scaleLayout->addWidget(new QLabel("Scale", this));

    xScaleInput = new QLineEdit(this);
    xScaleInput->setPlaceholderText("X");
    xScaleInput->setMaximumWidth(100);
    scaleLayout->addWidget(xScaleInput);

    yScaleInput = new QLineEdit(this);
    yScaleInput->setPlaceholderText("Y");
    yScaleInput->setMaximumWidth(100);
    scaleLayout->addWidget(yScaleInput);

    zScaleInput = new QLineEdit(this);
    zScaleInput->setPlaceholderText("Z");
    zScaleInput->setMaximumWidth(100);
    scaleLayout->addWidget(zScaleInput);
    connect(xScaleInput, &QLineEdit::returnPressed, this, &MainWindow::onScaleEntered);
    connect(yScaleInput, &QLineEdit::returnPressed, this, &MainWindow::onScaleEntered);
    connect(zScaleInput, &QLineEdit::returnPressed, this, &MainWindow::onScaleEntered);
    return scaleLayout;
}

void MainWindow::onCoordinateEntered() {
    bool okX, okY, okZ;

    QString xText = xCoordInput->text();
    QString yText = yCoordInput->text();
    QString zText = zCoordInput->text();

    float x = xText.isEmpty() ? 0.0f : xText.toFloat(&okX);
    float y = yText.isEmpty() ? 0.0f : yText.toFloat(&okY);
    float z = zText.isEmpty() ? 0.0f : zText.toFloat(&okZ);

    m_glWidget->SetObjectPosition(x, y, z);
}

void MainWindow::onRotationEntered()
{
    bool okX, okY, okZ;

    QString xText = xRotationInput->text();
    QString yText = yRotationInput->text();
    QString zText = zRotationInput->text();

    float x = xText.isEmpty() ? 0.0f : xText.toFloat(&okX);
    float y = yText.isEmpty() ? 0.0f : yText.toFloat(&okY);
    float z = zText.isEmpty() ? 0.0f : zText.toFloat(&okZ);

    m_glWidget->SetObjectRotation(x, y, z);
}

void MainWindow::onScaleEntered()
{
    bool okX, okY, okZ;

    QString xText = xScaleInput->text();
    QString yText = yScaleInput->text();
    QString zText = zScaleInput->text();

    float x = xText.isEmpty() ? 0.0f : xText.toFloat(&okX);
    float y = yText.isEmpty() ? 0.0f : yText.toFloat(&okY);
    float z = zText.isEmpty() ? 0.0f : zText.toFloat(&okZ);

    m_glWidget->SetObjectScale(x, y, z);
}

void MainWindow::LoadModel(const std::string& name)
{
    m_glWidget->loadModel(name);
}
