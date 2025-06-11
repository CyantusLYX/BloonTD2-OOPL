#ifndef LOADER_HPP
#define LOADER_HPP
#include "entities/bloon.hpp"
#include <nlohmann/json.hpp>
namespace loader {
std::vector<Bloon::Type> load_bloons(int wave);
}
#endif
