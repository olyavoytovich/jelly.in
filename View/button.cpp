#include "button.h"

ImageSet::ImageSet(const QString& name) {
  flat = QImage(":/images/menu/" + name + "_first.png");
  clicked = QImage(":/images/menu/" + name + "_second.png");
  hovered = QImage(":/images/menu/" + name + "_third.png");

  flat_scaled = flat;
  clicked_scaled = clicked;
  hovered_scaled = hovered;
}

Button::Button(std::shared_ptr<ImageSet> image_set,
               QWidget* parent,
               const QString& button_text)
    : QPushButton(parent),
      status_(Status::kFlat),
      image_set_(std::move(image_set)) {
  setMouseTracking(true);
  if (!button_text.isEmpty()) {
    SetText(button_text);
  }
  click_sound_.AddMedia("qrc:/sound/menu/click.mp3");
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
  click_sound_.Replay();
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

int Button::GetFontSize() const {
  if (text_ == nullptr) {
    return 0;
  }
  return text_->GetFontSize();
}

void Button::SetText(const QString& text) {
  if (text_ != nullptr) {
    text_->setText(text);
    return;
  }
  text_ = new Label(text, this);
  text_->setAlignment(Qt::AlignCenter);
  text_->SetFontColor(Qt::white);
}

void Button::SetFontSize(int text_size) {
  if (text_ != nullptr) {
    text_->SetFontSize(text_size);
  }
}

void Button::SetRectangle(const QRect& rectangle) {
  setGeometry(rectangle);
  if (text_ != nullptr) {
    int padding_x = static_cast<int>(width() * kTextPadding);
    int padding_y = static_cast<int>(height() * kTextPadding);

    // SetTextBounding используется только для установки размеров шрифта,
    // но не размеров самой Label.
    text_->SetTextBounding(QRect(padding_x, padding_y,
                                 width() - 2 * padding_x,
                                 height() - 2 * padding_y));
    text_->setGeometry(0, 0, width(), height());
  }
}
