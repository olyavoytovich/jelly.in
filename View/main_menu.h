#ifndef VIEW_MAIN_MENU_H_
#define VIEW_MAIN_MENU_H_

#include <QApplication>
#include <memory>

#include "choose_level_menu.h"
#include "menu.h"

class MainMenu : public Menu {
 public:
  MainMenu(QRect boundary_rectangle, AbstractGameController *game_controller,
           QWidget *parent = nullptr);

  void PressedButton(std::shared_ptr<Button> button) override;

 private:
  std::shared_ptr<Button> exit_button_;
  std::shared_ptr<Button> settings_button_;
  std::shared_ptr<Button> play_button_;
};

#endif  // VIEW_MAIN_MENU_H_
