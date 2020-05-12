#ifndef VIEW_MAIN_MENU_H_
#define VIEW_MAIN_MENU_H_

#include <QApplication>
#include <memory>

#include "menu.h"

class MainMenu : public Menu {
 public:
  explicit MainMenu(AbstractGameController* game_controller,
                    QWidget* parent = nullptr);
  ~MainMenu() override = default;

 private:
  void resizeEvent(QResizeEvent* event) override;

 private:
  Button* exit_button_;
  Button* settings_button_;
  Button* play_button_;
};

#endif  // VIEW_MAIN_MENU_H_
