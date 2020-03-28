#include "map_loader.h"

std::shared_ptr<Map> MapLoader::LoadMap(const QString& map_name) {
  QFile input_file(":/data/" + map_name + ".json");

  input_file.open(QIODevice::ReadOnly);
  QString file_text = input_file.readAll();
  input_file.close();

  QImage map_image(":/images/" + map_name + ".png");

  QJsonDocument json_document(QJsonDocument::fromJson(file_text.toUtf8()));
  QJsonObject json_main = json_document.object();
  QJsonArray solids = json_main["solids"].toArray();

  QJsonObject object, point_obj;

  auto map = std::make_shared<Map>(map_image);
  for (const auto& solid_object : solids) {
    object = solid_object.toObject();

    QPolygon object_polygon;
    if (!object["polygon"].isNull()) {
      // Add a polygon
      for (const auto& point : object["polygon"].toArray()) {
        point_obj = point.toObject();
        object_polygon << QPoint(object["x"].toInt() + point_obj["x"].toInt(),
                                 object["y"].toInt() + point_obj["y"].toInt());
      }
    } else {
      // Add a rectangle
      object_polygon = QRect(object["x"].toInt(),
                             object["y"].toInt(),
                             object["width"].toInt(),
                             object["height"].toInt());
    }
    map->AddGameObject(std::make_shared<GameObject>(object_polygon));
  }

  return map;
}
