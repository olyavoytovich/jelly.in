QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = jellyin
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17
QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
        entity.cpp \
        game_controller.cpp \
        game_object.cpp \
        main.cpp \
        map.cpp \
        map_loader.cpp \
        view.cpp \

        src/box2d/collision/b2_broad_phase.cpp \
        src/box2d/collision/b2_chain_shape.cpp \
        src/box2d/collision/b2_circle_shape.cpp \
        src/box2d/collision/b2_collide_circle.cpp \
        src/box2d/collision/b2_collide_edge.cpp \
        src/box2d/collision/b2_collide_polygon.cpp \
        src/box2d/collision/b2_collision.cpp \
        src/box2d/collision/b2_distance.cpp \
        src/box2d/collision/b2_dynamic_tree.cpp \
        src/box2d/collision/b2_edge_shape.cpp \
        src/box2d/collision/b2_polygon_shape.cpp \
        src/box2d/collision/b2_time_of_impact.cpp \
        src/box2d/common/b2_block_allocator.cpp \
        src/box2d/common/b2_draw.cpp \
        src/box2d/common/b2_math.cpp \
        src/box2d/common/b2_settings.cpp \
        src/box2d/common/b2_stack_allocator.cpp \
        src/box2d/common/b2_timer.cpp \
        src/box2d/dynamics/b2_body.cpp \
        src/box2d/dynamics/b2_chain_circle_contact.cpp \
        src/box2d/dynamics/b2_chain_polygon_contact.cpp \
        src/box2d/dynamics/b2_circle_contact.cpp \
        src/box2d/dynamics/b2_contact.cpp \
        src/box2d/dynamics/b2_contact_manager.cpp \
        src/box2d/dynamics/b2_contact_solver.cpp \
        src/box2d/dynamics/b2_distance_joint.cpp \
        src/box2d/dynamics/b2_edge_circle_contact.cpp \
        src/box2d/dynamics/b2_edge_polygon_contact.cpp \
        src/box2d/dynamics/b2_fixture.cpp \
        src/box2d/dynamics/b2_friction_joint.cpp \
        src/box2d/dynamics/b2_gear_joint.cpp \
        src/box2d/dynamics/b2_island.cpp \
        src/box2d/dynamics/b2_joint.cpp \
        src/box2d/dynamics/b2_motor_joint.cpp \
        src/box2d/dynamics/b2_mouse_joint.cpp \
        src/box2d/dynamics/b2_polygon_circle_contact.cpp \
        src/box2d/dynamics/b2_polygon_contact.cpp \
        src/box2d/dynamics/b2_prismatic_joint.cpp \
        src/box2d/dynamics/b2_pulley_joint.cpp \
        src/box2d/dynamics/b2_revolute_joint.cpp \
        src/box2d/dynamics/b2_rope_joint.cpp \
        src/box2d/dynamics/b2_weld_joint.cpp \
        src/box2d/dynamics/b2_wheel_joint.cpp \
        src/box2d/dynamics/b2_world.cpp \
        src/box2d/dynamics/b2_world_callbacks.cpp \
        src/box2d/rope/b2_rope.cpp

HEADERS += \
    abstract_game_controller.h \
    abstract_view.h \
    game_controller.h \
    game_object.h \
    entity.h \
    map.h \
    map_loader.h \
    view.h \
    
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

    src/box2d/dynamics/b2_chain_circle_contact.h \
    src/box2d/dynamics/b2_chain_polygon_contact.h \
    src/box2d/dynamics/b2_circle_contact.h \
    src/box2d/dynamics/b2_contact_solver.h \
    src/box2d/dynamics/b2_edge_circle_contact.h \
    src/box2d/dynamics/b2_edge_polygon_contact.h \
    src/box2d/dynamics/b2_island.h \
    src/box2d/dynamics/b2_polygon_circle_contact.h \
    src/box2d/dynamics/b2_polygon_contact.h

RESOURCES += \
    resources.qrc
