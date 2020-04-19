#ifndef MODEL_MAP_LOADER_H_
#define MODEL_MAP_LOADER_H_

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <memory>

#include "entity.h"
#include "map.h"
#include "patroller.h"
#include "shooter.h"

class MapLoader {
 public:
  // Returns a pointer to the loaded map, or nullptr if loading failed
  static std::shared_ptr<Map> LoadMap(const QString& map_name);
};

#endif  // MODEL_MAP_LOADER_H_
