#ifndef VIEW_GAME_INTERFACE_H_
#define VIEW_GAME_INTERFACE_H_

#include <memory>
#include <vector>

#include "menu.h"
#include "movie.h"

class GameInterface : public Menu {
 public:
  explicit GameInterface(AbstractGameController* game_controller,
                         QWidget* parent = nullptr);
  ~GameInterface() override = default;

  void SetHealth(int health);

 private:
  void resizeEvent(QResizeEvent* event) override;

 private:
  bool player_loosed_ = false;
  int current_health_ = 0;

  std::vector<std::shared_ptr<Movie>> health_;
};

#endif  // VIEW_GAME_INTERFACE_H_
