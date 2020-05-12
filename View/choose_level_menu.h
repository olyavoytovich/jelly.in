#ifndef VIEW_CHOOSE_LEVEL_MENU_H_
#define VIEW_CHOOSE_LEVEL_MENU_H_

#include <algorithm>
#include <memory>
#include <vector>

#include "menu.h"
#include "movie.h"

class ChooseLevelMenu : public Menu {
 public:
  explicit ChooseLevelMenu(AbstractGameController* game_controller,
                           QWidget* parent = nullptr);
  ~ChooseLevelMenu() override = default;

 private:
  void resizeEvent(QResizeEvent* event) override;
  void SetPlayerAnimation();
  void SetNextAnimation();

 private:
  const std::vector<QString> kPlayerAnimations = {"red", "orange", "yellow"};

 private:
  std::shared_ptr<Movie> player_ = nullptr;
  int player_animation_index_ = 0;

  Button* back_button_;
  std::vector<Button*> level_buttons_;
};

#endif  // VIEW_CHOOSE_LEVEL_MENU_H_
