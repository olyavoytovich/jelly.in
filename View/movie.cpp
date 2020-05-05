#include "movie.h"

Movie::Movie(const QString& name, QWidget* parent)
    : QWidget(parent),
      movie_(std::make_shared<QMovie>(":/images/gifs/" + name + ".gif")) {
  movie_->setCacheMode(QMovie::CacheMode::CacheAll);

  connect(movie_.get(), &QMovie::frameChanged, this, [&]() {
    UpdateScale();
  });
  connect(movie_.get(), &QMovie::finished, this, [&]() {
    Finished();
  });
  movie_->jumpToFrame(0);
}

void Movie::Play(bool is_looped) {
  is_looped_ = is_looped;
  is_finished_ = false;

  if (movie_->state() == QMovie::Paused) {
    movie_->setPaused(false);
  } else {
    movie_->start();
  }
}

void Movie::Pause() {
  movie_->setPaused(true);
}

void Movie::Stop() {
  movie_->stop();
}

void Movie::SetSpeed(int percents) {
  movie_->setSpeed(percents);
}

bool Movie::IsFinished() const {
  return is_finished_;
}

void Movie::resizeEvent(QResizeEvent* event) {
  QWidget::resizeEvent(event);
  UpdateScale();
}

void Movie::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  painter.drawImage(QPoint(0, 0), scaled_frames_[movie_->currentFrameNumber()]);
}

void Movie::UpdateScale() {
  int current_frame = movie_->currentFrameNumber();
  if (frames_.size() <= current_frame) {
    frames_.emplace_back(movie_->currentImage());
    scaled_frames_.emplace_back(movie_->currentImage());
  }
  if (scaled_frames_[current_frame].size() != size()) {
    scaled_frames_[current_frame] = frames_[current_frame].scaled(size());
  }
  repaint();
}

void Movie::Finished() {
  is_finished_ = true;
  if (is_looped_) {
    Play(true);
  }
}
