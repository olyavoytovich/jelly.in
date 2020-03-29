#ifndef VIEW_H_
#define VIEW_H_

#include <QMainWindow>
#include <QObject>
#include <QPaintEvent>
#include <QTimer>

#include "abstract_game_controller.h"
#include "abstract_view.h"

class View : public QMainWindow, public AbstractView {
 Q_OBJECT

 public:
  explicit View(AbstractGameController* game_controller);
  ~View() override = default;

 private:
  void paintEvent(QPaintEvent*) override;

 private:
  AbstractGameController* game_controller_ = nullptr;
  QTimer* timer_;
};

#endif  // VIEW_H_
