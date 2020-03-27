#include "map_loader.h"

std::shared_ptr<Map> MapLoader::LoadMap(const QString& map_name) {
  QFile input_file(":/data/" + map_name + ".json");
  if (!input_file.open(QIODevice::ReadOnly)) {
    // Cannot open map description file (.json)
    return std::shared_ptr<Map>();
  }

  auto image = new QImage(":/images/" + map_name + ".png");
  if (image->isNull()) {
    // Cannot open map image file
    return std::shared_ptr<Map>();
  }

  QString file_text = input_file.readAll();
  input_file.close();

  QJsonDocument json_document(QJsonDocument::fromJson(file_text.toUtf8()));
  QJsonObject json_main = json_document.object();
  QJsonArray layers = json_main["layers"].toArray();

  QJsonObject layer_obj, object, point_obj;

  auto map = std::make_shared<Map>(image);

  for (const auto& layer : layers) {
    if (!layer.isObject()) {
      continue;
    }
    layer_obj = layer.toObject();

    if (layer_obj["name"].toString() == "solids") {
      for (const auto& solid_object : layer_obj["objects"].toArray()) {
        object = solid_object.toObject();

        QPolygon object_polygon;
        if (!object["polygon"].isNull()) {
          // Add a polygon
          for (const auto& point : object["polygon"].toArray()) {
            point_obj = point.toObject();
            object_polygon << QPoint(static_cast<int>(object["x"].toDouble()
                                         + point_obj["x"].toDouble()),
                                     static_cast<int>(object["y"].toDouble()
                                         + point_obj["y"].toDouble()));
          }
        } else {
          // Add a rectangle
          object_polygon = QRect(static_cast<int>(object["x"].toDouble()),
                                 static_cast<int>(object["y"].toDouble()),
                                 static_cast<int>(object["width"].toDouble()),
                                 static_cast<int>(object["height"].toDouble()));
        }
        map->AddGameObject(std::make_shared<GameObject>(object_polygon));
      }
    }
  }

  return map;
}
