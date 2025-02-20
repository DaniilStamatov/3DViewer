#include <QIcon>
#include <QMouseEvent>
#include <QPixmap>
#include <QPushButton>
#include <QTimer>
#include <QTransform>

class ArrowButton : public QPushButton {
  Q_OBJECT

public:
  ArrowButton(const QString &iconPath, int rotationAngle,
              QWidget *parent = nullptr)
      : QPushButton(parent), holdTimer(new QTimer(this)) {
    setArrowIcon(iconPath, rotationAngle);
    setFixedSize(10, 10);
    setIconSize(size());
    connect(holdTimer, &QTimer::timeout, this, &ArrowButton::onHold);
  }

private slots:
  void onHold() { emit holdAction(); }

protected:
  void mousePressEvent(QMouseEvent *event) override {
    if (event->button() == Qt::LeftButton) {
      holdTimer->start(100);
      emit buttonPressed();
    }
    QPushButton::mousePressEvent(event);
  }

  void mouseReleaseEvent(QMouseEvent *event) override {
    if (event->button() == Qt::LeftButton) {
      holdTimer->stop();
      emit buttonReleased();
    }
    QPushButton::mouseReleaseEvent(event);
  }
signals:
  void buttonPressed();
  void buttonReleased();
  void holdAction();

private:
  void setArrowIcon(const QString &iconPath, int rotationAngle) {
    QPixmap originalPixmap(iconPath);

    QTransform transform;
    transform.rotate(rotationAngle);
    QPixmap rotatedPixmap = originalPixmap.transformed(transform);

    setIcon(QIcon(rotatedPixmap));
  }
  QTimer *holdTimer;
};