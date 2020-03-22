#ifndef VIEW_H_
#define VIEW_H_

#include <QMainWindow>

class GameController;
class QTimer;
class QPaintEvent;

class View : public QMainWindow {
  Q_OBJECT

 public:
  View();
  explicit View(GameController* game_controller);
  ~View();

 private slots:
  void paintEvent(QPaintEvent*);
  void Update();

 private:
  GameController* game_controller_;
  QTimer* timer_;
};

#endif  // VIEW_H_
