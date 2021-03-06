#ifndef VIEW_INTERMEDIATE_MENU_H_
#define VIEW_INTERMEDIATE_MENU_H_

#include <memory>
#include <vector>

#include "menu.h"
#include "movie.h"

enum class MenuType {
  kFail,
  kPause,
  kVictory,
  kControls
};

class IntermediateMenu : public Menu {
 public:
  explicit IntermediateMenu(AbstractGameController* game_controller,
                            MenuType menu_type,
                            QWidget* parent = nullptr);
  ~IntermediateMenu() override = default;

 private:
  void resizeEvent(QResizeEvent* event) override;

 private:
  MenuType menu_type_;

  Button* resume_button_ = nullptr;
  Button* restart_button_ = nullptr;
  Button* choose_level_button_ = nullptr;
  Button* main_menu_button_ = nullptr;
  Button* back_arrow_ = nullptr;

  std::shared_ptr<Movie> menu_animation_;
  std::vector<std::shared_ptr<Movie>> mushrooms_;

  std::vector<Button*> main_button_group_;
};

#endif  // VIEW_INTERMEDIATE_MENU_H_
