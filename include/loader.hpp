#ifndef LOADER_HPP
#define LOADER_HPP
#include <json/json.h>
#include "bloon.hpp"
namespace loader{
    std::vector<Bloon::Type> load_bloons(int wave);
}
#endif