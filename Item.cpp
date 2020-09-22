#include "Item.hpp"
#include "Platform.hpp"
#include "Load.hpp"

const float subsection_dist[] { 0.5f, 1.0f, 1.5f };
extern Scene::Drawable::Pipeline lit_color_texture_program_pipeline;
extern Load< MeshBuffer > ball_escape_meshes;

Item::Item(Type type, Platform* platform, int sector_index, int sub_index) :
    platform_(platform),
    existing_time_(0.0f),
    base_rotation_(glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f))),
    type_(type),
    sector_index_(sector_index),
    sub_index_(sub_index),
    rotation_(0.0f)
 {
    translation_transform_ = new Scene::Transform();
    rotation_transform_ = new Scene::Transform();
    if (type == Type::COIN) {
        base_relative_height_ = 0.5f;
        rotation_speed_ = 90.0f;
        floating_speed_ = glm::radians(90.0f);
        floating_amplitude_ = 0.1f;
        collision_radius_ = 0.15f;
        mesh_name_ = "Coin";
    } else {
        base_relative_height_ = 0.7f;
        rotation_speed_ = 90.0f;
        floating_speed_ = glm::radians(90.0f);
        floating_amplitude_ = 0.15f;
        collision_radius_ = 0.5f;
        mesh_name_ = "Heart";
    }
    
    rotation_transform_->parent = translation_transform_;
    float rou = subsection_dist[sub_index];
    float degree = glm::radians(10.0f + sector_index * 20.0f);
    translation_transform_->position[0] = rou * glm::cos(rou);
    translation_transform_->position[1] = rou * glm::sin(rou);
    
    Animate(0.0f);
}

void Item::Animate(float elapsed_time)
{
    existing_time_ += elapsed_time;
    translation_transform_->position[2] = platform_->height + base_relative_height_ + floating_amplitude_ * glm::sin(floating_speed_ * existing_time_);

    rotation_ += elapsed_time * rotation_speed_;
    if (rotation_ > 360.0f) {
        rotation_ -= (static_cast<int>(rotation_) / 360) * 360.0f;
    }

    rotation_transform_->rotation = glm::angleAxis(glm::radians(rotation_), glm::vec3(0.0f, 0.0f, 1.0f)) * base_rotation_;
}

bool Item::IsCollision(const glm::vec3& position, float radius)
{
    glm::vec2 ball_pos_2d(position[0], position[1]);
    glm::vec2 item_pos_2d(translation_transform_->position[0], translation_transform_->position[1]);

    return glm::distance(ball_pos_2d, item_pos_2d) < radius + collision_radius_;
}

void Item::ApplyEffect(PlayMode& playmode) 
{
    if (type_ == Type::COIN) {

    } else {

    }
}

void Item::Draw(Scene& scene)
{
    scene.drawables.emplace_back(rotation_transform_);
    auto &drawable = scene.drawables.back();
    drawable.pipeline = lit_color_texture_program_pipeline;
    drawable.mesh = &(ball_escape_meshes->lookup(mesh_name_));
}
