#pragma once

#include "biome.hpp"
#include "tile_type.hpp"

struct Floor_Info {
    sf::Vector2i coordinates;
    bool planted = false;
    Biome biome;
    Floor_Type floor;
    Detail_Type detail{ Detail_Type::NULL_TYPE };
    sf::Vector2i texture_pos;
    sf::Vector2i detail_pos;
};