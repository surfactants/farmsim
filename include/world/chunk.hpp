#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

#include "biome.hpp"
#include "tile.hpp"

class Chunk : public sf::Drawable{
public:
    Chunk() = default;
    Chunk(sf::Vector2i start , sf::Vector2i size , Map_Tile<Floor_Info>& info);

    bool contains(sf::Vector2f pos);
    bool contains(sf::Vector2i coords);

    Floor* getFloor(sf::Vector2i i);
    Detail* getDetail(sf::Vector2i i);

    void eraseDetail(sf::Vector2i i);

private:
    sf::FloatRect f_bounds;
    sf::IntRect i_bounds;
    sf::Vector2i start;
    sf::Vector2i end;
    sf::RectangleShape frame;
    Map_Tile<std::shared_ptr<Floor>> floor;
    Map_Tile<std::shared_ptr<Detail>> details;
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
