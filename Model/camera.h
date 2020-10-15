#ifndef MODEL_CAMERA_H_
#define MODEL_CAMERA_H_

#include <QPainter>
#include <QRect>

class Camera {
 public:
  explicit Camera(const QSize& map_size);

  void UpdateCamera(const QRect& window_rectangle,
                    const QPoint& player_position);

  void MovePainterToCamera(QPainter* painter) const;

  QSize GetCameraSize() const;
  double GetScale() const;

 private:
  void MoveCameraAfterPlayer(const QPoint& player_position);

 private:
  // Ширина и Высота камеры
  const QPoint kVisibleSize = QPoint(1600, 900);
  // Ширина и Высота прямоугольника игрока (из центра камеры)
  const QPoint kPlayerBoundary = QPoint(400, 225);

 private:
  QRect current_camera_;
  QSize map_size_;

  double scale_ = 1;
  QPoint shift_ = QPoint();

  QSize last_window_size = QSize();
};

#endif  // MODEL_CAMERA_H_
