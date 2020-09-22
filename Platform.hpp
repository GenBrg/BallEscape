//
// Created by hao on 9/20/20.
//

#include <vector>
#include "Sector.hpp"
#include "Item.hpp"

#ifndef INC_15_466_F20_BASE1_PLATFORM_H
#define INC_15_466_F20_BASE1_PLATFORM_H

/**
 * This defines a single platform (consists of 18 sectors)
 */
struct Platform {

    constexpr static double THICKNESS = 0.22378f;
    constexpr static double INNER_RADIUS = 1.9206f;
    constexpr static double CENTRAL_COLUMN_RADIUS = 0.204f;

    // deconstruct init speed
    constexpr static double DECONST_SPEED = 1.0f;

    // a vector of sectors (fix length 18)
    std::vector<Sector> sectors;

    std::vector<Item> items;

    // current height of this platform
    double height;

    // (if we want to implement rotate) we need to record the current angle of the rotation
    // in degree [0, 359]
    double rotate_degree;

    //
    bool start_deconstruct = false;
    bool finish_deconstruct = false;
    double time_since_deconstruct = 0.0f;

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

    /**
     * Get the type of board of a given position
     */
    Sector::SecType get_sec_type(double x, double y);
};

#endif //INC_15_466_F20_BASE1_PLATFORM_H
