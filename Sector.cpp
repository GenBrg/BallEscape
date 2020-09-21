#include "Sector.hpp"
#include "Load.hpp"

#include <string>

extern Load< MeshBuffer > ball_escape_meshes;
//
//void Sector::set_type(Type type, int sub_index) {
//    std::string mesh_name("Sector");
//
//    switch (type) {
//        case Type::NORMAL:
//            mesh_name += "Normal";
//        break;
//        case Type::HOLE:
//            mesh_name += "Hole";
//            mesh_name += ('1' + sub_index);
//        break;
//        case Type::RED:
//            mesh_name += "Red";
//            mesh_name += ('1' + sub_index);
//        break;
//        default:;
//    }
//
////    drawable->mesh = &(ball_escape_meshes->lookup(mesh_name));
//}
//
//std::string Sector::get_mesh_name() {
//    return std::string();
//}
