#include "Scene.hpp"
#include "Mesh.hpp"

struct Sector {
    enum class Type : uint8_t {
        NORMAL = 0,
        RED,
        HOLE
    };

    Scene::Drawable* drawable;

    Sector(Scene::Drawable* drawable_) : drawable(drawable_) {}
    void set_type(Type type_, int sub_index);
    void draw(Scene* scene);
};


