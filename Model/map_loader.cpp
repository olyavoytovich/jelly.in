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
  map->SetContactListener(std::make_shared<ContactListener>());
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
                                                object_points,
                                                EntityType::kGround));
  }

  QJsonArray dynamic_objects = json_main["dynamic_objects"].toArray();
  for (const auto& dynamic_object : dynamic_objects) {
    object = dynamic_object.toObject();

    if (object["name"].toString() == "player") {
      QPoint position(object["x"].toInt(), object["y"].toInt());
      QRect object_points(-Player::kPlayerWidth / 2,
                          -Player::kPlayerHeight / 2,
                          Player::kPlayerWidth,
                          Player::kPlayerHeight);
      map->SetPlayerObject(std::make_shared<Player>(map,
                                                    position,
                                                    object_points));
    }

    if (object["type"].isNull()) {
      continue;
    }

    QJsonArray way_point_array = object["way_points"].toArray();
    std::vector<QPoint> way_points(object["way_point_size"].toInt());
    for (int i = 0; i < way_points.size(); i++) {
      way_points[i] = QPoint(way_point_array[i].toObject()["x"].toInt(),
                             way_point_array[i].toObject()["y"].toInt());
    }

    QPoint object_position(object["x"].toInt(), object["y"].toInt());
    QPolygon object_points = QRect(-object["width"].toInt() / 2,
                                   -object["height"].toInt() / 2,
                                   object["width"].toInt(),
                                   object["height"].toInt());
    int object_speed = object["speed"].toInt();

    if (object["type"].toString() == "patroller") {
      map->AddGameObject(std::make_shared<Patroller>(map,
                                                     b2_dynamicBody,
                                                     object_position,
                                                     object_points,
                                                     way_points,
                                                     object_speed));
    }
    if (object["type"].toString() == "shooter") {
      BulletDirection bullet_direction = BulletDirection::kLeftRight;
      b2BodyType body_type = b2_dynamicBody;
      if (object["shoot_direction"].toString() == "down") {
        bullet_direction = BulletDirection::kBottom;
        body_type = b2_kinematicBody;
      }
      int shoot_period = object["shoot_period"].toInt();
      int bullet_speed = object["bullet_speed"].toInt();
      int bullet_radius = object["bullet_radius"].toInt();
      map->AddGameObject(std::make_shared<Shooter>(map,
                                                   body_type,
                                                   object_position,
                                                   object_points,
                                                   way_points,
                                                   bullet_direction,
                                                   shoot_period,
                                                   bullet_speed,
                                                   bullet_radius,
                                                   object_speed));
    }
  }

  return map;
}
