#include "Scene.hpp"
#include "Mesh.hpp"

struct Sector {
    enum class Type : uint8_t {
        NORMAL = 0,
        RED,
        HOLE
    } type;
    uint8_t red_position;
    Scene::Transform transform;

    Sector(Type type_, int red_position_ = 0) :
        type(type_), red_position(red_position_) {}

    void draw(Scene* scene);
};


