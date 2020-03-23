#ifndef VIEW_H_
#define VIEW_H_

#include <QObject>
#include <QPaintEvent>
#include <QTimer>
#include <memory>

#include "abstract_game_controller.h"
#include "abstract_view.h"

class View : public QMainWindow, public AbstractView {
  Q_OBJECT

 public:
  explicit View(AbstractGameController* game_controller);
  ~View() override = default;

 private:
  AbstractGameController* game_controller_ = nullptr;
  std::shared_ptr<QTimer> timer_;

 private:
  void paintEvent(QPaintEvent*) override;
};

#endif  // VIEW_H_
