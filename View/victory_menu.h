#ifndef VIEW_VICTORY_MENU_H_
#define VIEW_VICTORY_MENU_H_

#include <memory>
#include <vector>

#include "menu.h"
#include "movie.h"

class VictoryMenu : public Menu {
 public:
  explicit VictoryMenu(AbstractGameController* game_controller,
                       QWidget* parent = nullptr);
  ~VictoryMenu() override = default;

 private:
  void resizeEvent(QResizeEvent* event) override;

 private:
  Button* next_level_button_;
  Button* restart_button_;
  Button* choose_level_button_;
  Button* main_menu_button_;

  std::vector<Button*> main_button_group_;
};

#endif  // VIEW_VICTORY_MENU_H_
