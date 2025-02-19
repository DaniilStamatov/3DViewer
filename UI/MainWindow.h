#pragma once
#include "OpenGLWindow.h"

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget* parent = nullptr);
    void onCoordinateEntered();
    void onRotationEntered();
    void onScaleEntered();
    void LoadModel(const std::string& name);
    QHBoxLayout* CreateRotationLayout();
    QHBoxLayout* CreateTransformLayout();
    QHBoxLayout* CreateScaleLayout();
    QWidget* CreateButtonPanel();
private:
    OpenGLWindow* m_glWidget;
    QLineEdit *xCoordInput;
    QLineEdit *yCoordInput;
    QLineEdit *zCoordInput;
    QLineEdit *xRotationInput;
    QLineEdit *yRotationInput;
    QLineEdit *zRotationInput;
    QLineEdit *xScaleInput;
    QLineEdit *yScaleInput;
    QLineEdit *zScaleInput;
};