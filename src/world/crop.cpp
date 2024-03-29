#include <world/crop.hpp>

#include <util/prng.hpp>

const float Crop::stage_threshold = 100.f;

Crop::Crop(Crop_Data d)
    : Crop_Data(d)
{
}

void Crop::setSprite(sf::Sprite sprite)
{
    this->sprite = sprite;
}

sf::Sprite& Crop::getSprite()
{
    return sprite;
}

void Crop::tick(bool watered)
{
    // introduce random variance
    if (!fullyGrown() && !dead()) {
        float stage_advance = growth_coef;
        if (watered) {
            stage_advance *= water_factor;
        }
        //std::cout << "plant has advanced from " << growth;
        growth += stage_advance;
        //std::cout << " to " << growth << '\n';
        if (growth >= stage_threshold) {
            nextStage();
            unwater(coordinates);
        }
    }
}

sf::Vector2i Crop::getCoordinates()
{
    return coordinates;
}

bool Crop::fullyGrown()
{
    return (stage == stage_count);
}

size_t Crop::getUID()
{
    return uid;
}

size_t Crop::harvestUID()
{
    return uid + 1000;
}

void Crop::place(sf::Vector2i coordinates, sf::Vector2f pos)
{
    this->coordinates = coordinates;
    sprite.setPosition(pos);
}

void Crop::nextStage()
{
    sf::IntRect tr = sprite.getTextureRect();
    tr.top += y_size;
    sprite.setTextureRect(tr);
    growth = 0.f;
    stage++;
}

bool Crop::checkSeason(Season s)
{
    return seasons[s];
}

void Crop::kill()
{
    while (stage <= stage_count) {
        nextStage();
    }
}

bool Crop::dead()
{
    return (stage == stage_count + 1);
}

unsigned int Crop::getQuantity()
{
    return prng::number(quantity_min, quantity_max);
}

void Crop::harvestRegrowable()
{
    sf::IntRect tr = sprite.getTextureRect();
    tr.top -= y_size;
    sprite.setTextureRect(tr);
    growth = 0.f;
    stage--;
}

bool Crop::regrows()
{
    return m_regrows;
}

bool Crop::passable()
{
    return m_passable;
}

void Crop::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(sprite, states);
}
