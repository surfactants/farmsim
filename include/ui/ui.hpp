#pragma once

#include <SFML/Graphics.hpp>

#include <memory>

#include <game/game.hpp>

#include "ui_elements.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class UI : public sf::Drawable {
public:
    UI() = delete;

    UI(sf::RenderWindow& window, Game& game, sf::View& view);

    void scale(sf::RenderWindow& window);

    void update();

    void stopInput();

    void newLevel();

    void resize(sf::Vector2u windowSize);

    void scroll(float delta);

    void numRelease(int num);

    void init();

    void toggleInventory();
    void toggleMap();
    void closeOverlay();

    bool overlayActive();
    void setOverlay(bool active);

    bool clickLeft();
    bool releaseLeft();

    bool clickRight();
    bool releaseRight();

    void checkBuilding();

    void loadDefaultReactions();

    void loadInterface(Building* b);
    void closeInterface();

    void readSeasonChange();

private:
    sf::RenderWindow& window;
    Game& game;
    sf::Font& font;

    sf::View& view;

    sf::Text player_pos;

    sf::RectangleShape overlay;

    sf::RectangleShape season_display;

    std::unique_ptr<Inventory_Interface> inventory_interface;

    Player_Target player_target;

    bool overlay_active = false;

    Minimap minimap;

    int last_player_energy{ -1 };
    Energy_Bar energy_bar;

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};
