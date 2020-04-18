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

    QPolygon object_points;
    QPoint object_position = QPoint(object["x"].toInt(), object["y"].toInt());
    if (!object["polygon"].isNull()) {
      // Add a polygon
      for (const auto& point : object["polygon"].toArray()) {
        point_obj = point.toObject();
        object_points << QPoint(point_obj["x"].toInt(), point_obj["y"].toInt());
      }
    } else {
      // Add a rectangle
      object_points =
          QRect(0, 0, object["width"].toInt(), object["height"].toInt());
    }
    map->AddGameObject(std::make_shared<Entity>(map,
                                                b2_staticBody,
                                                object_position,
                                                object_points));
  }

  return map;
}
