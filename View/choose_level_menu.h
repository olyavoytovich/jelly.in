#ifndef VIEW_CHOOSE_LEVEL_MENU_H_
#define VIEW_CHOOSE_LEVEL_MENU_H_

#include <memory>
#include <vector>

#include "menu.h"

class ChooseLevelMenu : public Menu {
 public:
  explicit ChooseLevelMenu(AbstractGameController* game_controller,
                           QWidget* parent = nullptr);
  ~ChooseLevelMenu() override = default;

  void PressedButton(const std::shared_ptr<Button>& button) override;

 private:
  std::shared_ptr<Button> back_button_;

  std::vector<std::shared_ptr<Button>> buttons_;
};

#endif  // VIEW_CHOOSE_LEVEL_MENU_H_
