//
// Created by hao on 9/20/20.
//

#include <vector>
#include "Sector.hpp"
#include <random>

#ifndef INC_15_466_F20_BASE1_PLATFORM_H
#define INC_15_466_F20_BASE1_PLATFORM_H

/**
 * This defines a single platform (consists of 18 sectors)
 */
struct Platform {
    // a vector of sectors (fix length 18)
    std::vector<Sector> sectors;

    // current height of this platform
    double height;

    // (if we want to implement rotate) we need to record the current angle of the rotation
    // in degree [0, 359]
    double rotate_degree;

    /**
     * @param hole_cnt how many holes in this platform
     * @param red_cnt how many reds in this platform
     */
    Platform(int hole_cnt, int red_cnt, double height_);

    /**
     * Get a transform for sectors[i]
     * Note: remember to delete
     */
    Scene::Transform* get_transform(size_t idx);
};

#endif //INC_15_466_F20_BASE1_PLATFORM_H
