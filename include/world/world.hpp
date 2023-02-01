#pragma once

#include <SFML/Graphics.hpp>

#include <map>
#include <memory>
#include <variant>

#include <entity/player.hpp>

#include <item/item.hpp>
#include <item/item_library.hpp>
#include <item/player_inventory.hpp>

#include "crop.hpp"
#include "crop_library.hpp"

#include "biome.hpp"
#include "chunk.hpp"
#include "chunk_loader.hpp"
#include "tile.hpp"
#include "tile_info.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class World : public sf::Drawable {
public:
    World(Item_Library& item_library);

    void reset();
    void makeBiomes();
    void makeGrass();
    void initialAutotile();
    void finalize(sf::Vector2i player_coordinates);

    Map_Tile<Crop>& getCrops();

    std::vector<sf::FloatRect> getLocalImpassableTiles(sf::Vector2i p);

    void update(Player_Inventory& inventory, sf::Vector2i player_coordinates);

    sf::Vector2i* checkMouseTarget(sf::Vector2f mpos, sf::Vector2i playerCoords);

    void useItem(Item* item);

    void useTool(Item* item);
    void hoe();
    void water();

    void plantCrop(Item* item);

    void tick();

    void setInteracting(bool interacting);

    void interact(Player_Inventory& inventory);

    sf::Vector2i posToCoords(sf::Vector2f pos);

    sf::Vector2i worldMin();
    sf::Vector2i worldMax();

    Map_Tile<Floor_Info>& getTileLibrary();

    Floor* activeFloor(sf::Vector2i i);

private:
    bool changeActiveTile(Floor_Type prereq, Floor_Type ntype);

    Item_Library& item_library;
    Crop_Library crop_library;

    Map_Tile<Floor_Info> tile_library;
    Chunk_Loader chunks { tile_library };

    sf::Vector2i world_min;
    sf::Vector2i world_max;

    bool interacting = false;

    Map_Tile<Crop> crops;

    bool inRange(sf::Vector2i c1, sf::Vector2i c2);

    sf::Clock tickClock;

    std::unique_ptr<sf::Vector2i> activeTile;

    void updateGrass(sf::Vector2i start, sf::Vector2i end);
    int autotileX(sf::Vector2i i, std::variant<Biome, Detail_Type> type);
    int autotileX(bool n, bool w, bool s, bool e);
    bool adjacentDetailMatch(sf::Vector2i i, Detail_Type type);
    bool adjacentBiomeMatch(sf::Vector2i i, Biome type);
    bool validLibraryTile(int x, int y);

    void tileToLibrary(sf::Vector2i i);
    void tileToLibrary(Floor* f);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
