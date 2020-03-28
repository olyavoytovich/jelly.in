#ifndef MAP_LOADER_H_
#define MAP_LOADER_H_

#include <memory>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include "map.h"

class MapLoader {
 public:
  // Returns a pointer to the loaded map, or nullptr if loading failed
  static std::shared_ptr<Map> LoadMap(const QString& map_name);
};

#endif  // MAP_LOADER_H_
