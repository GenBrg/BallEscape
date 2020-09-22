#pragma once

#include "Scene.hpp"

struct Platform;
struct PlayMode;

class Item {
private:
    Platform* platform_;
    Scene::Transform* translation_transform_ = nullptr;
    Scene::Transform* rotation_transform_ = nullptr;
    float existing_time_;

    float base_relative_height_;
    glm::quat base_rotation_;;
    float rotation_speed_;
    float floating_speed_;
    float floating_amplitude_;
    float collision_radius_;

    std::string mesh_name_;

    float rotation_;

public:
    enum class Type : uint8_t {
        COIN,
        HEART
    } type_;

    Item(Type type, Platform* platform, int sector_index, int sub_index);
    void Animate(float elapsed_time);
    void Draw(Scene& scene);
    bool IsCollision(const glm::vec3& position, float radius);
    void ApplyEffect(PlayMode& playmode);
    ~Item() {
        // if (translation_transform_)
        //     delete translation_transform_;
        // if (rotation_transform_)
        //     delete rotation_transform_;
    }
};
