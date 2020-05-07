#ifndef VIEW_FAIL_MENU_H_
#define VIEW_FAIL_MENU_H_

#include <algorithm>
#include <memory>

#include "menu.h"
#include "button.h"

class FailMenu : public Menu {
 public:
  explicit FailMenu(AbstractGameController* game_controller,
                    QWidget* parent = nullptr);
  ~FailMenu() override = default;

 private:
  void resizeEvent(QResizeEvent* event) override;

 private:
  Button* restart_button_;
  Button* choose_level_button_;
  Button* main_menu_button_;
};

#endif  // VIEW_FAIL_MENU_H_
