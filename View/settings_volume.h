#ifndef VIEW_SETTINGS_VOLUME_H_
#define VIEW_SETTINGS_VOLUME_H_

#include <memory>
#include <vector>

#include "menu.h"

class SettingsVolume : public Menu {
 public:
  explicit SettingsVolume(AbstractGameController* game_controller,
                          QWidget* parent = nullptr);
  ~SettingsVolume() override = default;

 private:
  void resizeEvent(QResizeEvent* event) override;

 private:
  Button* back_arrow_;

  std::vector<Button*> right_arrows_;
  std::vector<Button*> left_arrows_;
  std::vector<Button*> squares_;
};

#endif  // VIEW_SETTINGS_VOLUME_H_
