//
// Created by hao on 9/20/20.
//

#include "Platform.hpp"
#include "Sector.hpp"

static int SECS_PER_PLATFORM = 18;

Platform::Platform(int hole_cnt, int red_cnt, double height_): height(height_), rotate_degree(0.0f) {
    int hole_left = hole_cnt;
    int red_left = red_cnt;
    int normal_left = SECS_PER_PLATFORM - hole_cnt - red_cnt;
    for(int i=0; i<SECS_PER_PLATFORM; i++) {
        int rand_val = rand() % (hole_left + red_left + normal_left);
        Sector::SecType secType;
        if(rand_val < hole_left) {
            secType = Sector::SecType::HOLE;
            hole_left--;
        } else if (rand_val < hole_left + red_left) {
            secType = Sector::SecType::RED;
            red_left--;
        } else {
            secType = Sector::SecType::NORMAL;
            normal_left--;
        }
        sectors.push_back(
                Sector(secType,secType == Sector::SecType::HOLE ? 1 + random() % 2 : random() % 3));
    }
}

Scene::Transform *Platform::get_transform(int idx) {
    Scene::Transform* transform_p = new Scene::Transform();
    // calculate the transform based on current height/this sector's relative position
    transform_p->position[2] += height; // becomes higher

    // use angle axis to compute location of this sector
    transform_p->rotation = transform_p->rotation * glm::angleAxis(
            glm::radians(idx * 360.0f/SECS_PER_PLATFORM),
            glm::vec3(0.0f, 0.0f, 1.0f));

    return transform_p;
}
