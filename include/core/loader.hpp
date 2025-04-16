#ifndef LOADER_HPP
#define LOADER_HPP
#include <nlohmann/json.hpp>
#include "entities/bloon.hpp"
namespace loader{
    std::vector<Bloon::Type> load_bloons(int wave);
}
#endif