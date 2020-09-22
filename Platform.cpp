//
// Created by hao on 9/20/20.
//

#include "Platform.hpp"
#include "Sector.hpp"

#include <random>
#define M_PI 3.14159265358

static int SECS_PER_PLATFORM = 18;

Platform::Platform(int hole_cnt, int red_cnt, double height_): height(height_), rotate_degree(0.0f) {
    int hole_left = hole_cnt;
    int red_left = red_cnt;
    int normal_left = SECS_PER_PLATFORM - hole_cnt - red_cnt;

    int items_left = 5;

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
            if (items_left > 0) {
                // 30% chance spawn an item
                if ((rand() % 10) > 6) {
                    int subsec_idx = rand() % 3;
                    if (rand() % 10 > 0) {
                        // 90% chance spawn a coin
                        items.emplace_back(Item::Type::COIN, this, i, subsec_idx);
                    } else {
                        // 10% chance spawn a coin
                        items.emplace_back(Item::Type::HEART, this, i, subsec_idx);
                    }
                    --items_left;
                }
            }    
        }
        
        Sector sec = Sector(secType,secType == Sector::SecType::HOLE ? 1 + rand() % 2 : rand() % 3);
        sectors.push_back(sec);
//        std::cout<<"idx = "<<i<<" type="<<(int)sec.type<<" sub_idx="<<sec.sub_index<<std::endl;
    }
}

Scene::Transform *Platform::get_transform(size_t idx) {
    Scene::Transform* transform_p = new Scene::Transform();
    // calculate the transform based on current height/this sector's relative position
    transform_p->position[2] += static_cast<float>(height); // becomes higher

    float deg_per_sec = 360.0f / SECS_PER_PLATFORM;

    // use angle axis to compute location of this sector
    transform_p->rotation *= glm::angleAxis(
            glm::radians((idx + 13.0f) * deg_per_sec + deg_per_sec/2),
            glm::vec3(0.0f, 0.0f, 1.0f));

    return transform_p;
}

Sector::SecType Platform::get_sec_type(double x, double y) {
    //convert to polar coordinates
    double r = sqrt(x*x + y*y);
    double degrees = fmod(atan2(y, x) / (2 * M_PI) * 360 + 360, 360);

    float deg_per_sec = 360.0f / SECS_PER_PLATFORM;
    int idx = degrees / deg_per_sec;
    Sector::SecType type = Sector::SecType::NORMAL;

    if (r < Sector::INNER_RANGE && sectors[idx].sub_index == 0) {
        type = sectors[idx].type;
    } else if(r < Sector::MID_RANGE && r > Sector::INNER_RANGE && sectors[idx].sub_index == 1) {
        type = sectors[idx].type;
    } else if(r < Sector::OUTTER_RANGE && r > Sector::MID_RANGE && sectors[idx].sub_index == 2) {
        type = sectors[idx].type;
    }
//
//    std::cout<<"x="<<x<<" y="<<y<<"\n";
//    std::cout<<"idx="<<idx<<" r="<<r<<" deg="<<degrees<<" type="<<(int)type<<std::endl;

    return type;
}
