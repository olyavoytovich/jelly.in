#ifndef CHOOSE_LEVEL_MENU_H_
#define CHOOSE_LEVEL_MENU_H_

#include "menu.h"

class ChooseLevelMenu : public Menu {
 public:
  ChooseLevelMenu(QRect boundary_rectangle,
                  AbstractGameController *game_controller,
                  QWidget *parent = nullptr);

  void PressedButton(std::shared_ptr<Button> button) override;

 private:
  std::shared_ptr<Button> back_button_;

  std::shared_ptr<Button> first_level_button_;
  std::shared_ptr<Button> second_level_button_;
  std::shared_ptr<Button> third_level_button_;
  std::shared_ptr<Button> fourth_level_button_;
  std::shared_ptr<Button> fifth_level_button_;
  std::shared_ptr<Button> sixth_level_button_;
  std::shared_ptr<Button> seventh_level_button_;
  std::shared_ptr<Button> eighth_level_button_;
  std::shared_ptr<Button> ninth_level_button_;
  std::shared_ptr<Button> tenth_level_button_;
  std::shared_ptr<Button> eleventh_level_button_;
  std::shared_ptr<Button> tvelvth_level_button_;
};

#endif  // CHOOSE_LEVEL_MENU_H_
