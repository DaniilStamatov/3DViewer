#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);
  QHBoxLayout *layout = new QHBoxLayout(centralWidget);

  m_glWidget = new OpenGLWindow(this);
  layout->addWidget(m_glWidget);

  QWidget *buttonPanel = CreateButtonPanel();
  layout->addWidget(buttonPanel);
}

QWidget *MainWindow::CreateButtonPanel() {
  QWidget *buttonPanel = new QWidget(this);
  QVBoxLayout *settingsLayout = new QVBoxLayout(buttonPanel);

  settingsLayout->addWidget(new QLabel("Transform", this));
  settingsLayout->addLayout(CreateTransformLayout());
  settingsLayout->addLayout(CreateRotationLayout());
  settingsLayout->addLayout(CreateScaleLayout());
  QSpacerItem *spacer =
      new QSpacerItem(20, 200, QSizePolicy::Minimum, QSizePolicy::Expanding);
  settingsLayout->addItem(spacer);

  buttonPanel->setMaximumWidth(300);
  return buttonPanel;
}

QVBoxLayout *MainWindow::createArrowButtons(const QString &iconPath,
                                            void (MainWindow::*upSlot)(),
                                            void (MainWindow::*downSlot)()) {
  QVBoxLayout *buttonsLayout = new QVBoxLayout();
  buttonsLayout->setSpacing(1);

  ArrowButton *upButton = new ArrowButton(iconPath, -90);
  ArrowButton *downButton = new ArrowButton(iconPath, 90);

  buttonsLayout->addWidget(upButton);
  buttonsLayout->addWidget(downButton);

  connect(upButton, &ArrowButton::buttonPressed, this, upSlot);
  connect(upButton, &ArrowButton::holdAction, this, upSlot);
  connect(downButton, &ArrowButton::holdAction, this, downSlot);
  connect(downButton, &ArrowButton::buttonPressed, this, downSlot);

  return buttonsLayout;
}

QHBoxLayout *MainWindow::CreateTransformLayout() {
  QHBoxLayout *transformLayout = new QHBoxLayout();
  transformLayout->addWidget(new QLabel("Position", this));

  transformLayout->addLayout(createArrowButtons(
      "res/images/arrow.png", &MainWindow::onXUpButtonClicked,
      &MainWindow::onXDownButtonClicked));
  transformLayout->addSpacerItem(
      new QSpacerItem(30, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
  xCoordInput = new QLineEdit(this);
  xCoordInput->setPlaceholderText("X");
  xCoordInput->setMaximumWidth(100);
  transformLayout->addWidget(xCoordInput);

  transformLayout->addLayout(createArrowButtons(
      "res/images/arrow.png", &MainWindow::onYUpButtonClicked,
      &MainWindow::onYDownButtonClicked));
  yCoordInput = new QLineEdit(this);
  yCoordInput->setPlaceholderText("Y");
  yCoordInput->setMaximumWidth(100);
  transformLayout->addWidget(yCoordInput);

  transformLayout->addLayout(createArrowButtons(
      "res/images/arrow.png", &MainWindow::onZUpButtonClicked,
      &MainWindow::onZDownButtonClicked));
  zCoordInput = new QLineEdit(this);
  zCoordInput->setPlaceholderText("Z");
  zCoordInput->setMaximumWidth(100);
  transformLayout->addWidget(zCoordInput);

  connect(xCoordInput, &QLineEdit::returnPressed, this,
          &MainWindow::onCoordinateEntered);
  connect(yCoordInput, &QLineEdit::returnPressed, this,
          &MainWindow::onCoordinateEntered);
  connect(zCoordInput, &QLineEdit::returnPressed, this,
          &MainWindow::onCoordinateEntered);
  transformLayout->setSpacing(5);
  return transformLayout;
}

QHBoxLayout *MainWindow::CreateRotationLayout() {
  QHBoxLayout *rotationLayout = new QHBoxLayout();
  rotationLayout->addWidget(new QLabel("Rotation", this));
  rotationLayout->addLayout(createArrowButtons(
      "res/images/arrow.png", &MainWindow::onXRotUpButtonClicked,
      &MainWindow::onXRotDownButtonClicked));
  xRotationInput = new QLineEdit(this);
  xRotationInput->setPlaceholderText("X");
  xRotationInput->setMaximumWidth(100);
  rotationLayout->addWidget(xRotationInput);
  rotationLayout->addLayout(createArrowButtons(
      "res/images/arrow.png", &MainWindow::onYRotUpButtonClicked,
      &MainWindow::onYRotDownButtonClicked));
  yRotationInput = new QLineEdit(this);
  yRotationInput->setPlaceholderText("Y");
  yRotationInput->setMaximumWidth(100);
  rotationLayout->addWidget(yRotationInput);
  rotationLayout->addLayout(createArrowButtons(
      "res/images/arrow.png", &MainWindow::onZRotUpButtonClicked,
      &MainWindow::onZRotDownButtonClicked));
  zRotationInput = new QLineEdit(this);
  zRotationInput->setPlaceholderText("Z");
  zRotationInput->setMaximumWidth(100);
  rotationLayout->addWidget(zRotationInput);
  connect(xRotationInput, &QLineEdit::returnPressed, this,
          &MainWindow::onRotationEntered);
  connect(yRotationInput, &QLineEdit::returnPressed, this,
          &MainWindow::onRotationEntered);
  connect(zRotationInput, &QLineEdit::returnPressed, this,
          &MainWindow::onRotationEntered);
  rotationLayout->setSpacing(5);
  return rotationLayout;
}

QHBoxLayout *MainWindow::CreateScaleLayout() {
  QHBoxLayout *scaleLayout = new QHBoxLayout();
  scaleLayout->addWidget(new QLabel("Scale", this));
  scaleLayout->addLayout(createArrowButtons(
      "res/images/arrow.png", &MainWindow::onXScaleUpButtonClicked,
      &MainWindow::onXScaleDownButtonClicked));
  scaleLayout->addSpacerItem(
      new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

  xScaleInput = new QLineEdit(this);
  xScaleInput->setPlaceholderText("X");
  xScaleInput->setMaximumWidth(100);
  scaleLayout->addWidget(xScaleInput);
  scaleLayout->addLayout(createArrowButtons(
      "res/images/arrow.png", &MainWindow::onYScaleUpButtonClicked,
      &MainWindow::onYScaleDownButtonClicked));
  yScaleInput = new QLineEdit(this);
  yScaleInput->setPlaceholderText("Y");
  yScaleInput->setMaximumWidth(100);
  scaleLayout->addWidget(yScaleInput);
  scaleLayout->addLayout(createArrowButtons(
      "res/images/arrow.png", &MainWindow::onZScaleUpButtonClicked,
      &MainWindow::onZScaleDownButtonClicked));
  zScaleInput = new QLineEdit(this);
  zScaleInput->setPlaceholderText("Z");
  zScaleInput->setMaximumWidth(100);
  scaleLayout->addWidget(zScaleInput);
  connect(xScaleInput, &QLineEdit::returnPressed, this,
          &MainWindow::onScaleEntered);
  connect(yScaleInput, &QLineEdit::returnPressed, this,
          &MainWindow::onScaleEntered);
  connect(zScaleInput, &QLineEdit::returnPressed, this,
          &MainWindow::onScaleEntered);
  scaleLayout->setSpacing(5);
  return scaleLayout;
}

void MainWindow::onCoordinateEntered() { SetObjectPosition(); }

void MainWindow::onRotationEntered() { SetObjectRotation(); }

void MainWindow::onScaleEntered() { SetObjectScale(); }

void MainWindow::LoadModel(const std::string &name) {
  m_glWidget->loadModel(name);
}

void MainWindow::SetObjectPosition() {
  bool okX, okY, okZ;

  QString xText = xCoordInput->text();
  QString yText = yCoordInput->text();
  QString zText = zCoordInput->text();

  float x = xText.isEmpty() ? 0.0f : xText.toFloat(&okX);
  float y = yText.isEmpty() ? 0.0f : yText.toFloat(&okY);
  float z = zText.isEmpty() ? 0.0f : zText.toFloat(&okZ);

  m_glWidget->SetObjectPosition(x, y, z);
}

void MainWindow::SetObjectRotation() {
  bool okX, okY, okZ;

  QString xText = xRotationInput->text();
  QString yText = yRotationInput->text();
  QString zText = zRotationInput->text();

  float x = xText.isEmpty() ? 0.0f : xText.toFloat(&okX);
  float y = yText.isEmpty() ? 0.0f : yText.toFloat(&okY);
  float z = zText.isEmpty() ? 0.0f : zText.toFloat(&okZ);

  m_glWidget->SetObjectRotation(x, y, z);
}

void MainWindow::SetObjectScale() {
  bool okX, okY, okZ;

  QString xText = xScaleInput->text();
  QString yText = yScaleInput->text();
  QString zText = zScaleInput->text();

  float x = xText.isEmpty() ? 0.0f : xText.toFloat(&okX);
  float y = yText.isEmpty() ? 0.0f : yText.toFloat(&okY);
  float z = zText.isEmpty() ? 0.0f : zText.toFloat(&okZ);

  m_glWidget->SetObjectScale(x, y, z);
}

void MainWindow::onXUpButtonClicked() { updateCoordinate(xCoordInput, 1); }

void MainWindow::onXDownButtonClicked() { updateCoordinate(xCoordInput, -1); }

void MainWindow::onYUpButtonClicked() { updateCoordinate(yCoordInput, 1); }

void MainWindow::onYDownButtonClicked() { updateCoordinate(yCoordInput, -1); }

void MainWindow::onZDownButtonClicked() { updateCoordinate(zCoordInput, -1); }
void MainWindow::onZUpButtonClicked() { updateCoordinate(zCoordInput, 1); }

void MainWindow::onXScaleUpButtonClicked() { updateScale(xScaleInput, 0.1); }
void MainWindow::onXScaleDownButtonClicked() { updateScale(xScaleInput, -0.1); }
void MainWindow::onYScaleUpButtonClicked() { updateScale(yScaleInput, 0.1); }
void MainWindow::onYScaleDownButtonClicked() { updateScale(yScaleInput, -0.1); }
void MainWindow::onZScaleDownButtonClicked() { updateScale(zScaleInput, -0.1); }

void MainWindow::onZScaleUpButtonClicked() { updateScale(zScaleInput, 0.1); }
void MainWindow::onXRotUpButtonClicked() { updateRotation(xRotationInput, 1); }
void MainWindow::onXRotDownButtonClicked() {
  updateRotation(xRotationInput, -1);
}

void MainWindow::onYRotDownButtonClicked() {
  updateRotation(yRotationInput, -1);
}

void MainWindow::onYRotUpButtonClicked() { updateRotation(yRotationInput, 1); }

void MainWindow::onZRotUpButtonClicked() { updateRotation(zRotationInput, 1); }

void MainWindow::onZRotDownButtonClicked() {
  updateRotation(zRotationInput, -1);
}

void MainWindow::updateCoordinate(QLineEdit *coordInput, int delta) {
  bool ok;
  int currentValue = coordInput->text().toInt(&ok);
  if (ok) {
    currentValue += delta;
    coordInput->setText(QString::number(currentValue));
  } else {
    coordInput->setText(QString::number(delta));
  }
  SetObjectPosition();
}

void MainWindow::updateRotation(QLineEdit *rotationInput, float delta) {
  bool ok;
  float currentValue = rotationInput->text().toFloat(&ok);
  if (ok) {
    currentValue += delta;
    rotationInput->setText(QString::number(currentValue));
  } else {
    rotationInput->setText(QString::number(delta));
  }
  SetObjectRotation();
}

void MainWindow::updateScale(QLineEdit *scaleInput, float delta) {
  bool ok;
  float currentValue = scaleInput->text().toFloat(&ok);
  if (ok) {
    currentValue += delta;
    scaleInput->setText(QString::number(currentValue));
  } else {
    scaleInput->setText(QString::number(delta));
  }
  SetObjectScale();
}
