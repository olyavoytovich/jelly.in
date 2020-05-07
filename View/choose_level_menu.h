#ifndef VIEW_CHOOSE_LEVEL_MENU_H_
#define VIEW_CHOOSE_LEVEL_MENU_H_

#include <algorithm>
#include <memory>
#include <vector>

#include "menu.h"

class ChooseLevelMenu : public Menu {
 public:
  explicit ChooseLevelMenu(AbstractGameController* game_controller,
                           QWidget* parent = nullptr);
  ~ChooseLevelMenu() override = default;

 private:
  void resizeEvent(QResizeEvent* event) override;

 private:
  Button* back_button_;
  std::vector<Button*> level_buttons_;
};

#endif  // VIEW_CHOOSE_LEVEL_MENU_H_
