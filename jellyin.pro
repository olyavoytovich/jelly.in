QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = jellyin
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += resources_big
CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
        Controller/game_controller.cpp \
        main.cpp \
        Model/animation.cpp \
        Model/animator.cpp \
        Model/audio_manager.cpp \
        Model/camera.cpp \
        Model/contact_listener.cpp \
        Model/entity.cpp \
        Model/game_object.cpp \
        Model/map.cpp \
        Model/map_loader.cpp \
        Model/patroller.cpp \
        Model/player.cpp \
        Model/pressure_plate.cpp \
        Model/shooter.cpp \
        View/button.cpp \
        View/choose_level_menu.cpp \
        View/game_interface.cpp \
        View/intermediate_menu.cpp \
        View/label.cpp \
        View/main_menu.cpp \
        View/menu.cpp \
        View/movie.cpp \
        View/settings_menu.cpp \
        View/settings_volume.cpp \
        View/view.cpp \
        box2d/collision/b2_broad_phase.cpp \
        box2d/collision/b2_chain_shape.cpp \
        box2d/collision/b2_circle_shape.cpp \
        box2d/collision/b2_collide_circle.cpp \
        box2d/collision/b2_collide_edge.cpp \
        box2d/collision/b2_collide_polygon.cpp \
        box2d/collision/b2_collision.cpp \
        box2d/collision/b2_distance.cpp \
        box2d/collision/b2_dynamic_tree.cpp \
        box2d/collision/b2_edge_shape.cpp \
        box2d/collision/b2_polygon_shape.cpp \
        box2d/collision/b2_time_of_impact.cpp \
        box2d/common/b2_block_allocator.cpp \
        box2d/common/b2_draw.cpp \
        box2d/common/b2_math.cpp \
        box2d/common/b2_settings.cpp \
        box2d/common/b2_stack_allocator.cpp \
        box2d/common/b2_timer.cpp \
        box2d/dynamics/b2_body.cpp \
        box2d/dynamics/b2_chain_circle_contact.cpp \
        box2d/dynamics/b2_chain_polygon_contact.cpp \
        box2d/dynamics/b2_circle_contact.cpp \
        box2d/dynamics/b2_contact.cpp \
        box2d/dynamics/b2_contact_manager.cpp \
        box2d/dynamics/b2_contact_solver.cpp \
        box2d/dynamics/b2_distance_joint.cpp \
        box2d/dynamics/b2_edge_circle_contact.cpp \
        box2d/dynamics/b2_edge_polygon_contact.cpp \
        box2d/dynamics/b2_fixture.cpp \
        box2d/dynamics/b2_friction_joint.cpp \
        box2d/dynamics/b2_gear_joint.cpp \
        box2d/dynamics/b2_island.cpp \
        box2d/dynamics/b2_joint.cpp \
        box2d/dynamics/b2_motor_joint.cpp \
        box2d/dynamics/b2_mouse_joint.cpp \
        box2d/dynamics/b2_polygon_circle_contact.cpp \
        box2d/dynamics/b2_polygon_contact.cpp \
        box2d/dynamics/b2_prismatic_joint.cpp \
        box2d/dynamics/b2_pulley_joint.cpp \
        box2d/dynamics/b2_revolute_joint.cpp \
        box2d/dynamics/b2_rope_joint.cpp \
        box2d/dynamics/b2_weld_joint.cpp \
        box2d/dynamics/b2_wheel_joint.cpp \
        box2d/dynamics/b2_world.cpp \
        box2d/dynamics/b2_world_callbacks.cpp \
        box2d/rope/b2_rope.cpp \
        Model/sound.cpp \
        Model/mushroom.cpp

HEADERS += \
    Controller/abstract_game_controller.h \
    Controller/game_controller.h \
    Model/animation.h \
    Model/animator.h \
    Model/audio_manager.h \
    Model/camera.h \
    Model/constants.h \
    Model/contact_listener.h \
    Model/entity.h \
    Model/game_object.h \
    Model/map.h \
    Model/map_loader.h \
    Model/patroller.h \
    Model/player.h \
    Model/pressure_plate.h \
    Model/shooter.h \
    View/abstract_view.h \
    View/button.h \
    View/choose_level_menu.h \
    View/game_interface.h \
    View/intermediate_menu.h \
    View/label.h \
    View/main_menu.h \
    View/menu.h \
    View/movie.h \
    View/settings_menu.h \
    View/settings_volume.h \
    View/view.h \
    box2d/b2_block_allocator.h \
    box2d/b2_body.h \
    box2d/b2_broad_phase.h \
    box2d/b2_chain_shape.h \
    box2d/b2_circle_shape.h \
    box2d/b2_collision.h \
    box2d/b2_contact.h \
    box2d/b2_contact_manager.h \
    box2d/b2_distance.h \
    box2d/b2_distance_joint.h \
    box2d/b2_draw.h \
    box2d/b2_dynamic_tree.h \
    box2d/b2_edge_shape.h \
    box2d/b2_fixture.h \
    box2d/b2_friction_joint.h \
    box2d/b2_gear_joint.h \
    box2d/b2_growable_stack.h \
    box2d/b2_joint.h \
    box2d/b2_math.h \
    box2d/b2_motor_joint.h \
    box2d/b2_mouse_joint.h \
    box2d/b2_polygon_shape.h \
    box2d/b2_prismatic_joint.h \
    box2d/b2_pulley_joint.h \
    box2d/b2_revolute_joint.h \
    box2d/b2_rope.h \
    box2d/b2_rope_joint.h \
    box2d/b2_settings.h \
    box2d/b2_shape.h \
    box2d/b2_stack_allocator.h \
    box2d/b2_time_of_impact.h \
    box2d/b2_time_step.h \
    box2d/b2_timer.h \
    box2d/b2_weld_joint.h \
    box2d/b2_wheel_joint.h \
    box2d/b2_world.h \
    box2d/b2_world_callbacks.h \
    box2d/box2d.h \
    box2d/dynamics/b2_chain_circle_contact.h \
    box2d/dynamics/b2_chain_polygon_contact.h \
    box2d/dynamics/b2_circle_contact.h \
    box2d/dynamics/b2_contact_solver.h \
    box2d/dynamics/b2_edge_circle_contact.h \
    box2d/dynamics/b2_edge_polygon_contact.h \
    box2d/dynamics/b2_island.h \
    box2d/dynamics/b2_polygon_circle_contact.h \
    box2d/dynamics/b2_polygon_contact.h \
    Model/sound.h \
    Model/mushroom.h

RESOURCES += \
    resources.qrc

VERSION = 0.1
