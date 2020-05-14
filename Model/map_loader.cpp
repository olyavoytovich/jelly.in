#include "map_loader.h"

std::shared_ptr<Map> MapLoader::LoadMap(const QString& map_name) {
  QFile input_file(":/data/" + map_name + ".json");

  if (!input_file.open(QIODevice::ReadOnly)) {
    return nullptr;
  }
  QString file_text = input_file.readAll();
  input_file.close();

  QImage map_image(":/images/" + map_name + ".png");
  if (map_image.isNull()) {
    return nullptr;
  }

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
      // Добавляет полигон
      for (const auto& point : object["polygon"].toArray()) {
        point_obj = point.toObject();
        object_points << QPoint(point_obj["x"].toInt(), point_obj["y"].toInt());
      }
    } else {
      // Добавляет прямоугольник
      object_points = QRect(-object["width"].toInt() / 2,
                            -object["height"].toInt() / 2,
                            object["width"].toInt(),
                            object["height"].toInt());
    }

    EntityType entity_type = EntityType::kGround;
    if (!object["type"].isNull()) {
      entity_type = EntityType::kSpikes;
    }
    map->AddGameObject(std::make_shared<Entity>(map,
                                                b2_staticBody,
                                                object_position,
                                                object_points,
                                                entity_type));
  }

  std::map<QString, std::shared_ptr<Animation>> name_to_animation;
  std::map<QString, std::shared_ptr<PressurePlate>> name_to_plate;

  if (!json_main["plates"].isNull()) {
    QJsonArray plates = json_main["plates"].toArray();
    for (const auto& plate : plates) {
      object = plate.toObject();
      QPoint position(object["x"].toInt(), object["y"].toInt());
      QPolygon object_points = QRect(-object["width"].toInt() / 2,
                                     -object["height"].toInt() / 2,
                                     object["width"].toInt(),
                                     object["height"].toInt());

      QJsonArray animations = object["animations"].toArray();

      std::shared_ptr<Animator>
          animator = CreateAnimator(&name_to_animation, animations);

      auto pressure_plate =
          std::make_shared<PressurePlate>(map,
                                          b2_staticBody,
                                          position,
                                          QPolygon(object_points),
                                          animator);
      name_to_plate[object["name"].toString()] = pressure_plate;
      map->AddGameObject(pressure_plate);
    }
  }

  QJsonArray dynamic_objects = json_main["dynamic_objects"].toArray();
  for (const auto& dynamic_object : dynamic_objects) {
    object = dynamic_object.toObject();

    if (object["name"].toString() == "exit") {
      QPoint position(object["x"].toInt(), object["y"].toInt());
      QRect object_points(-5, -5, 10, 10);
      map->AddGameObject(std::make_shared<Entity>(map,
                                                  b2_staticBody,
                                                  position,
                                                  QPolygon(object_points),
                                                  EntityType::kExit));
      continue;
    }

    if (object["animation_count"].isNull()) {
      continue;
    }

    QJsonArray animations = object["animations"].toArray();

    if (object["name"].toString() == "player") {
      QPoint position(object["x"].toInt(), object["y"].toInt());
      QRect object_points(-Player::kPlayerWidth / 2,
                          -Player::kPlayerHeight / 2,
                          Player::kPlayerWidth,
                          Player::kPlayerHeight);

      std::map<QString, std::shared_ptr<Animation>> name_to_player_animation;
      for (const auto& animation_name : kPlayerAnimations) {
        CreateAnimation(&name_to_player_animation, 1, 100, animation_name);
        for (const auto& current_animation : animations) {
          QJsonObject animation = current_animation.toObject();
          CreateAnimation(&name_to_player_animation,
                          animation["frames_count"].toInt(),
                          animation["duration"].toInt(),
                          animation["name"].toString() + "_" + animation_name);
        }
      }
      auto animator = std::make_shared<Animator>(name_to_player_animation,
                                                 kPlayerAnimations[0]);

      map->SetPlayerObject(std::make_shared<Player>(map,
                                                    position,
                                                    object_points,
                                                    animator));
      continue;
    }
    std::shared_ptr<Animator>
        animator = CreateAnimator(&name_to_animation, animations);

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

    if (object["type"].toString() == "platform") {
      auto game_object = std::make_shared<Entity>(map,
                                                  b2_kinematicBody,
                                                  object_position,
                                                  object_points,
                                                  EntityType::kGround);
      game_object->SetAnimator(animator);
      game_object->SetWayPoints(way_points);
      game_object->SetSpeed(object["speed"].toInt());
      name_to_plate[object["plate_name"].toString()]->AddPlatform(game_object);
      map->AddGameObject(game_object);
    }

    if (object["type"].toString() == "patroller") {
      if (!object["ellipse"].isNull()) {
        int radius = object["width"].toInt() / 2;
        map->AddGameObject(std::make_shared<Patroller>(map,
                                                       b2_dynamicBody,
                                                       object_position,
                                                       radius,
                                                       way_points,
                                                       animator,
                                                       object_speed));
      } else {
        map->AddGameObject(std::make_shared<Patroller>(map,
                                                       b2_dynamicBody,
                                                       object_position,
                                                       object_points,
                                                       way_points,
                                                       animator,
                                                       object_speed));
      }
    }

    if (object["type"].toString() == "shooter") {
      QJsonArray bullet_animations = object["bullet_animations"].toArray();
      std::shared_ptr<Animator> bullet_animator =
          CreateAnimator(&name_to_animation, bullet_animations);

      BulletDirection bullet_direction = BulletDirection::kLeftRight;
      b2BodyType body_type = b2_dynamicBody;
      if (object["shoot_direction"].toString() == "down") {
        bullet_direction = BulletDirection::kBottom;
        body_type = b2_kinematicBody;
      }
      int shoot_period = object["shoot_period"].toInt();
      int bullet_speed = object["bullet_speed"].toInt();
      int bullet_radius = object["bullet_radius"].toInt();

      EntityType shooter_type = EntityType::kDefault;
      QString animation_name = animations[0].toObject()["name"].toString();
      if (animation_name == "sunflower") {
        shooter_type = EntityType::kSunflower;
      } else if (animation_name == "cloud") {
        shooter_type = EntityType::kCloud;
      } else if (animation_name == "burdock") {
        shooter_type = EntityType::kBurdock;
      }

      if (shooter_type == EntityType::kDefault) {
        std::cout << "Error loading animation " << animation_name.toStdString();
        return nullptr;
      }

      map->AddGameObject(std::make_shared<Shooter>(map,
                                                   body_type,
                                                   object_position,
                                                   object_points,
                                                   way_points,
                                                   bullet_direction,
                                                   shoot_period,
                                                   bullet_speed,
                                                   bullet_radius,
                                                   animator,
                                                   bullet_animator,
                                                   shooter_type,
                                                   object_speed));
    }
  }
  return map;
}

std::shared_ptr<Animator> MapLoader::CreateAnimator(
    std::map<QString, std::shared_ptr<Animation>>* name_to_animation,
    const QJsonArray& animations) {
  std::map<QString, std::shared_ptr<Animation>> name_to_object_animation;
  for (const auto& animation : animations) {
    QJsonObject current_animation = animation.toObject();
    QString animation_name = current_animation["name"].toString();
    CreateAnimation(name_to_animation,
                    current_animation["frames_count"].toInt(),
                    current_animation["duration"].toInt(),
                    animation_name);
    name_to_object_animation[animation_name] =
        (*name_to_animation)[animation_name];
  }
  auto animator = std::make_shared<Animator>(
      name_to_object_animation,
      animations[0].toObject()["name"].toString());
  return animator;
}

void MapLoader::CreateAnimation(
    std::map<QString, std::shared_ptr<Animation>>* name_to_animation,
    int frames_count,
    int animation_duration,
    const QString& animation_name) {
  if (name_to_animation->find(animation_name) != name_to_animation->end()) {
    return;
  }
  std::vector<std::shared_ptr<QImage>> frames;
  for (int frame = 0; frame < frames_count; frame++) {
    frames.emplace_back(std::make_shared<QImage>(
        ":/images/animation/" + animation_name + QString::number(frame)
            + ".png"));
  }
  (*name_to_animation)[animation_name] = std::make_shared<Animation>(
      frames, animation_duration);
}
