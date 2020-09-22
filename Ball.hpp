//
// Created by hao on 9/21/20.
//

#include <deque>
#include <iostream>
#include "Scene.hpp"

#ifndef INC_15_466_F20_BASE1_BALL_H
#define INC_15_466_F20_BASE1_BALL_H

struct Ball {
    constexpr const static double RADIUS = 0.25f;
    constexpr const static double FRIC_ACC = -0.5f; // acc speed
    constexpr const static double COLLIDE_SPEED_RATIO = 0.5f; // loss 1 - COLLIDE_SPEED_RATIO speed for each collision

    // this ball's transform
    Scene::Transform* transform;

    //which platform holds the ball
    Platform *platform_p;

    Scene::Drawable* drawable_p;


    Ball() {
        transform = new Scene::Transform();
    }

    ~Ball() {
        delete transform;
    }

    bool is_falling = false;
    double time_since_fall = 0.0f;

private:

    // all current forces that apply to this ball
    glm::vec2 speed = glm::vec2(0, 0);


    void fade(double delta_v) {
        // delta_v is a negative
        if(fabs(speed[0]) + delta_v < 0) {
            speed[0] = 0;
        } else {
            speed[0] = speed[0] > 0 ? speed[0] + delta_v : speed[0] - delta_v;
        }
        if(fabs(speed[1]) + delta_v < 0) {
            speed[1] = 0;
        } else {
            speed[1] = speed[1] > 0 ? speed[1] + delta_v : speed[1] - delta_v;
        }
    }

public:


    void roll(double elapsed) {
        if(is_falling) {
            fall(elapsed);
            return;
        }

        fade(elapsed * FRIC_ACC); // delta v = a * t
        double delta_x = speed[0] * elapsed;
        double delta_y = speed[1] * elapsed;
        transform->position[0] += delta_x;
        transform->position[1] += delta_y;

        // when hitting the central column or the outer_wall
        double dist = sqrt(pow(transform->position[1], 2) + pow(transform->position[0], 2));

        if (dist < Platform::CENTRAL_COLUMN_RADIUS + Ball::RADIUS || dist > Platform::INNER_RADIUS - Ball::RADIUS) {
//            std::cout<<"x="<<transform->position[0]<<" y="<<transform->position[1]<<"\n";
//            std::cout<<"dist="<<dist<<" out="<<Platform::INNER_RADIUS - Ball::RADIUS<<" in="<<Platform::CENTRAL_COLUMN_RADIUS + Ball::RADIUS <<"\n";

            // calculate reflect speed when hit the wall or hit the central column
            glm::vec2 norm = glm::normalize(glm::vec2(transform->position[0], transform->position[1]));
            glm::vec2 tang = glm::normalize(glm::vec2(-transform->position[1], transform->position[0]));
            glm::vec2 speed_norm = - glm::dot(norm, speed) * norm; // norm
            glm::vec2 speed_tangent = glm::dot(tang, speed) * tang; // tangent
            speed[0] = speed_norm[0] + speed_tangent[0] * COLLIDE_SPEED_RATIO;
            speed[1] = speed_norm[1] + speed_tangent[1] * COLLIDE_SPEED_RATIO;

            // make sure it is still within the range
            if (dist > Platform::INNER_RADIUS - Ball::RADIUS) {
                glm::vec2 correct_cor = (float(dist) * 0.99f) * glm::normalize(glm::vec2(transform->position[0], transform->position[1]));
                transform->position[0] = correct_cor[0];
                transform->position[1] = correct_cor[1];
            } else {
                glm::vec2 correct_cor = (float(dist) * 1.01f) * glm::normalize(glm::vec2(transform->position[0], transform->position[1]));
                transform->position[0] = correct_cor[0];
                transform->position[1] = correct_cor[1];
            }

//            std::cout<<"reflected fixed: ";
//            std::cout<<"x="<<transform->position[0]<<" y="<<transform->position[1]<<"\n";

        }

        transform->position[2] = platform_p->height + Platform::THICKNESS + Ball::RADIUS;

        //delta_distances
        double distance = sqrt(delta_x * delta_x + delta_y * delta_y);
        //rotation axis
        if(delta_y != 0 || delta_x != 0) {
            glm::vec3 rot_axis(glm::normalize(glm::vec3(-delta_y, delta_x, 0)));
            transform->rotation = transform->rotation * glm::angleAxis((float) distance / (float) RADIUS, rot_axis);
        }
    }

    void fall(double elapsed) {
        time_since_fall += elapsed;
        transform->position[2] -= (time_since_fall * 9.8) * elapsed;
        double delta_x = speed[0] * elapsed;
        double delta_y = speed[1] * elapsed;
        transform->position[0] += delta_x;
        transform->position[1] += delta_y;

        // when hitting the central column or the outer_wall
        double dist = sqrt(pow(transform->position[1], 2) + pow(transform->position[0], 2));

        if (dist < Platform::CENTRAL_COLUMN_RADIUS + Ball::RADIUS) {
            // calculate reflect speed when  hit the central column
            glm::vec2 norm = glm::normalize(glm::vec2(transform->position[0], transform->position[1]));
            glm::vec2 tang = glm::normalize(glm::vec2(-transform->position[1], transform->position[0]));
            glm::vec2 speed_norm = - glm::dot(norm, speed) * norm; // norm
            glm::vec2 speed_tangent = glm::dot(tang, speed) * tang; // tangent
            speed[0] = speed_norm[0] + speed_tangent[0] * COLLIDE_SPEED_RATIO;
            speed[1] = speed_norm[1] + speed_tangent[1] * COLLIDE_SPEED_RATIO;

            // make sure it is still within the range
            glm::vec2 correct_cor = (float(dist) * 0.99f) * glm::normalize(glm::vec2(transform->position[0], transform->position[1]));
            transform->position[0] = correct_cor[0];
            transform->position[1] = correct_cor[1];
        }
    }

    void hit(glm::vec2 speed_) {
        speed[0] += speed_[0];
        speed[1] += speed_[1];
    }


    bool enter_hole() {
        return !is_falling && platform_p->get_sec_type(transform->position[0], transform->position[1]) == Sector::SecType::HOLE;
    }

    void reset_speed() {
        speed = glm::vec2(0, 0);
    }


};

#endif