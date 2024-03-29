#include <item/item_library.hpp>

#include <resources/texture_manager.hpp>

#include <system/database.hpp>

#include <util/vector2_stream.hpp>

Item_Library::Item_Library()
{
    std::vector<Item_Data> items = Database::getItemPrototypes();
    for (auto& item : items) {
        Item i(item);
        i.setCount(1);
        sf::Sprite sprite;
        sf::IntRect textureRect;

        std::string texture;

        switch (item.uid / 1000) {
            case 0:
                texture = "ITEMS";
                break;
            case 1:
                texture = "SEEDS";
                break;
            case 2:
                texture = "PLANTS";
                break;
            case 3:
                texture = "RAW_MATERIAL";
                break;
            case 4:
                texture = "CONTAINER";
                break;
            case 5:
                texture = "CRAFTING";
                break;
            case 6:
                texture = "MACHINE";
                break;
            case 7:
                texture = "LOOTABLE";
                break;
            case 8:
                texture = "FURNITURE";
                break;
            case 9:
                texture = "STRUCTURE";
                break;
            case 10:
                texture = "VEHICLE";
                break;
            default:
                texture = "ITEMS";
                break;
        }

        size_t sheet_id = item.uid % 1000;

        texture += std::to_string(sheet_id / 100);

        sheet_id %= 100;

        sprite.setTexture(Texture_Manager::get(texture));

        sf::Vector2i pos;
        sf::Vector2i size(32, 32);
        pos.x = (sheet_id % 10) * size.x;
        pos.y = (sheet_id / 10) * size.x;
        sprite.setTextureRect(sf::IntRect(pos, size));
        i.setSprite(sprite);

        std::shared_ptr<Item> iptr = std::make_shared<Item>(i);

        string_shelf[item.name] = iptr;
        uid_shelf[item.uid] = iptr;

        std::cout << "item " << item.name << " (" << item.uid << ") added, subtype " << item.subtype
                  << ", use factor " << item.use_factor
                  << " from sheet " << texture << " ("
                  << pos << " x " << size << ")\n";
    }
}
