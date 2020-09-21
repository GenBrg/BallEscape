

#include "Scene.hpp"
#include "Mesh.hpp"
#include <iostream>
#include <string>

#ifndef INC_15_466_F20_BASE1_SECTOR_H
#define INC_15_466_F20_BASE1_SECTOR_H


struct Sector {
    constexpr static double INNER_RANGE = 0.72233f;
    constexpr static double MID_RANGE = 1.3447f;
    constexpr static double OUTTER_RANGE = 1.9206f;

    enum class SecType : uint8_t {
        NORMAL = 0,
        RED,
        HOLE
    } type;

    // 0, 1 or 2 (the inner, mid, outer portion of this sector)
    int sub_index;

    // get the mesh name of this shape of sector
    std::string get_mesh_name() const {
        std::string mesh_name("Sector");

        switch (type) {
            case SecType::NORMAL:
                mesh_name += "Normal";
                break;
            case SecType::HOLE:
                mesh_name += "Hole";
                mesh_name += char('1' + sub_index);
                break;
            case SecType::RED:
                mesh_name += "Red";
                mesh_name += char('1' + sub_index);
                break;
            default:
                assert(false);
        }

        return mesh_name;
    }

    Sector(SecType type_, int sub_index_): type(type_), sub_index(sub_index_) {
        assert(sub_index >= 0 && sub_index <= 2);
    }

    Sector(): type(SecType::NORMAL), sub_index(0) {
        assert(sub_index >= 0 && sub_index <= 2);
    }
};

#endif

