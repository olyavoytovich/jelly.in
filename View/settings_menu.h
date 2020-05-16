#ifndef VIEW_SETTINGS_MENU_H_
#define VIEW_SETTINGS_MENU_H_

#include <memory>

#include "menu.h"
#include "movie.h"

class SettingsMenu : public Menu {
 public:
  explicit SettingsMenu(AbstractGameController* game_controller,
                        QWidget* parent = nullptr);
  ~SettingsMenu() override = default;

 private:
  void resizeEvent(QResizeEvent* event) override;

 private:
  Button* back_arrow_;
  Button* controls_;
  Button* volume_;

  std::shared_ptr<Movie> menu_animation_;
};

#endif  // VIEW_SETTINGS_MENU_H_
