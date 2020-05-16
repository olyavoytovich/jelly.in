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
  void SetMushrooms(int mushrooms);

 private:
  void resizeEvent(QResizeEvent* event) override;

 private:
  const int kMaximumHealth = 3;
  const int kMushroomsCount = 3;

 private:
  bool player_lost_ = false;
  int current_health_ = 0;
  int current_mushrooms_ = 0;

  std::vector<std::shared_ptr<Movie>> health_;
  std::vector<std::shared_ptr<Movie>> mushrooms_;
};

#endif  // VIEW_GAME_INTERFACE_H_
