#ifndef VIEW_MOVIE_H_
#define VIEW_MOVIE_H_

#include <QMovie>
#include <QPainter>
#include <QPushButton>
#include <memory>
#include <vector>

class Movie : public QPushButton {
 public:
  explicit Movie(const QString& name, QWidget* parent = nullptr);
  ~Movie() override = default;

  void Play(bool is_looped = false);
  void Pause();
  void Stop();
  void SetSpeed(int percents = 100);
  void SetAnimation(const QString& name);

  bool IsFinished() const;

 private:
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent*) override;

  void UpdateScale();
  void Finished();

 private:
  std::shared_ptr<QMovie> movie_;
  std::vector<QImage> scaled_frames_;

  // Принимает true, если анимацию надо постоянно повторять
  bool is_looped_ = false;
  bool is_finished_ = false;
};

#endif  // VIEW_MOVIE_H_
