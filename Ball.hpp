//
// Created by hao on 9/21/20.
//

#include <deque>
#include "Platform.hpp"
#include <iostream>

#ifndef INC_15_466_F20_BASE1_BALL_H
#define INC_15_466_F20_BASE1_BALL_H

struct Ball {
    // which platform it belongs to
    Platform* platform;

    // this ball's transform
    Scene::Transform* transform;

    double x = 0;
    double y = 0;

    // all current forces that apply to this ball
    std::deque<glm::vec2> speeds;

    void hit(glm::vec2 speed) {
        speeds.push_back(speed);
    }

    void fade(double delta_v) {
        // delta_v is a negative
        for(auto& f: speeds) {
            if(fabs(f[0]) + delta_v < 0) {
                f[0] = 0;
            } else {
                f[0] = f[0] > 0 ? f[0] + delta_v : f[0] - delta_v;
            }
            if(fabs(f[1]) + delta_v < 0) {
                f[1] = 0;
            } else {
                f[1] = f[1] > 0 ? f[1] + delta_v : f[1] - delta_v;
            }
        }

        // delete all force that already reaches 0
        while(!speeds.empty() && speeds.front()[0] == 0 && speeds.front()[1] == 0) {
            speeds.pop_front();
        }
    }

    glm::vec2 get_speed() {
        glm::vec2 res(0, 0);
        for(auto & f: speeds) {
            res[0] += f[0];
            res[1] += f[1];
        }
        return res;
    }

};

#endif //INC_15_466_F20_BASE1_BALL_H
