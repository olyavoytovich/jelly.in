#include "button.h"

ImageSet::ImageSet(const QString& name) {
  flat = QImage(":/images/menu/" + name + "_first.png");
  clicked = QImage(":/images/menu/" + name + "_second.png");
  hovered = QImage(":/images/menu/" + name + "_third.png");

  flat_scaled = flat;
  clicked_scaled = clicked;
  hovered_scaled = hovered;
}

Button::Button(std::shared_ptr<ImageSet> image_set, QWidget* parent)
    : QPushButton(parent),
      status_(Status::kFlat),
      image_set_(std::move(image_set)) {
  setMouseTracking(true);
}

void Button::paintEvent(QPaintEvent*) {
  QPainter painter(this);
  if (rect() != image_set_->flat_scaled.rect()) {
    image_set_->flat_scaled = image_set_->flat.scaled(rect().size());
    image_set_->clicked_scaled = image_set_->clicked.scaled(rect().size());
    image_set_->hovered_scaled = image_set_->hovered.scaled(rect().size());
  }

  switch (status_) {
    case Status::kFlat: {
      painter.drawImage(QPoint(0, 0), image_set_->flat_scaled);
      break;
    }
    case Status::kClicked: {
      painter.drawImage(QPoint(0, 0), image_set_->clicked_scaled);
      break;
    }
    case Status::kHovered: {
      painter.drawImage(QPoint(0, 0), image_set_->hovered_scaled);
      break;
    }
  }
}

void Button::mousePressEvent(QMouseEvent* event) {
  status_ = Status::kClicked;
  repaint();
  QPushButton::mousePressEvent(event);
}

void Button::mouseReleaseEvent(QMouseEvent* event) {
  if (underMouse()) {
    status_ = Status::kHovered;
  } else {
    status_ = Status::kFlat;
  }
  repaint();
  QPushButton::mouseReleaseEvent(event);
}

void Button::resizeEvent(QResizeEvent*) {
  repaint();
}

void Button::enterEvent(QEvent*) {
  status_ = Status::kHovered;
  repaint();
}

void Button::leaveEvent(QEvent*) {
  if (status_ != Status::kClicked) {
    status_ = Status::kFlat;
  }
  repaint();
}
