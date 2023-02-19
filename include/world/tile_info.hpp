#pragma once

#include <memory>

#include "biome.hpp"
#include "building.hpp"
#include "tile_type.hpp"
#include "tree.hpp"

struct Floor_Info {
    sf::Vector2i coordinates;
    bool planted = false;
    Biome biome{ Biome::NULL_TYPE };
    Floor_Type floor{ Floor_Type::NULL_TYPE };
    Detail_Type detail{ Detail_Type::NULL_TYPE };
    sf::Vector2i texture_pos;
    sf::Vector2i detail_pos;
    bool tree = false;
    bool rock = false;
    std::shared_ptr<Building> building = nullptr;
};
