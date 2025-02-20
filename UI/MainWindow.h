#pragma once
#include "ArrowButton.h"
#include "OpenGLWindow.h"
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow {
public:
  MainWindow(QWidget *parent = nullptr);
  void LoadModel(const std::string &name);
  QHBoxLayout *CreateRotationLayout();
  QHBoxLayout *CreateTransformLayout();
  QHBoxLayout *CreateScaleLayout();
  QWidget *CreateButtonPanel();
private slots:
  void onXUpButtonClicked();
  void onXDownButtonClicked();
  void onYUpButtonClicked();
  void onYDownButtonClicked();
  void onZUpButtonClicked();
  void onZDownButtonClicked();

  void onXRotUpButtonClicked();
  void onXRotDownButtonClicked();
  void onYRotUpButtonClicked();
  void onYRotDownButtonClicked();
  void onZRotUpButtonClicked();
  void onZRotDownButtonClicked();

  void onXScaleUpButtonClicked();
  void onXScaleDownButtonClicked();
  void onYScaleUpButtonClicked();
  void onYScaleDownButtonClicked();
  void onZScaleUpButtonClicked();
  void onZScaleDownButtonClicked();
  void onCoordinateEntered();
  void onRotationEntered();
  void onScaleEntered();
  QVBoxLayout *createArrowButtons(const QString &iconPath,
                                  void (MainWindow::*upSlot)(),
                                  void (MainWindow::*downSlot)());

private:
  void updateCoordinate(QLineEdit *coordInput, int delta);
  void updateRotation(QLineEdit *rotationInput, float delta);
  void updateScale(QLineEdit *rotationInput, float delta);
  OpenGLWindow *m_glWidget;
  QLineEdit *xCoordInput;
  QLineEdit *yCoordInput;
  QLineEdit *zCoordInput;
  QLineEdit *xRotationInput;
  QLineEdit *yRotationInput;
  QLineEdit *zRotationInput;
  QLineEdit *xScaleInput;
  QLineEdit *yScaleInput;
  QLineEdit *zScaleInput;
  void SetObjectPosition();
  void SetObjectRotation();
  void SetObjectScale();
};