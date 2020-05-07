#ifndef VIEW_PAUSE_MENU_H_
#define VIEW_PAUSE_MENU_H_

#include <algorithm>
#include <memory>

#include "menu.h"
#include "movie.h"

class PauseMenu : public Menu {
 public:
  explicit PauseMenu(AbstractGameController* game_controller,
                     QWidget* parent = nullptr);
  ~PauseMenu() override = default;

 private:
  void resizeEvent(QResizeEvent* event) override;

 private:
  Button* resume_button_;
  Button* restart_button_;
  Button* choose_level_button_;
  Button* main_menu_button_;

  std::shared_ptr<Movie> burdock_;
};

#endif  // VIEW_PAUSE_MENU_H_
