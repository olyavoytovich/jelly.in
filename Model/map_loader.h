#ifndef MODEL_MAP_LOADER_H_
#define MODEL_MAP_LOADER_H_

#include <QFile>
#include <QImage>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <memory>
#include <vector>

#include "animator.h"
#include "animation.h"
#include "Controller/abstract_game_controller.h"
#include "entity.h"
#include "map.h"
#include "patroller.h"
#include "player.h"
#include "shooter.h"

class MapLoader {
 public:
  // Returns a pointer to the loaded map, or nullptr if loading failed
  static std::shared_ptr<Map> LoadMap(const QString& map_name);

 private:
  static void CreateAnimation(
      std::map<QString, std::shared_ptr<Animation>>* name_to_animation,
      const QJsonObject& objects,
      const QString& animation_name);
};

#endif  // MODEL_MAP_LOADER_H_
