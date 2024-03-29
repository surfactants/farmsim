#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include <functional>
#include <map>

#include <game/game.hpp>

#include <menu/menu.hpp>

#include <system/season_changer.hpp>
#include <system/state_hook.hpp>

#include <ui/ui.hpp>

#include "action.hpp"
#include "input_package.hpp"
#include "mouse_event.hpp"

/////////////////////////////////////////////////////////////
/// \brief
///
class Input_Handler : private State_Hook {
public:
    Input_Handler(sf::RenderWindow& nwindow, Game& ngame, UI& nui, Menu_Package m, Season_Changer& season_changer);

    void handle();

    void menuChange();

    const std::map<std::string, Action> getRemappableActions();

    void setRemappableActions(const std::vector<Action>& actions);

private:
    sf::RenderWindow& window;

    sf::Event event;

    std::map<Main_State, Input_Package> context;
    std::map<Menu_State, Input_Package> context_menu;

    const bool& game_input_enabled;

    void placeActionTrigger(const std::pair<std::string, Action_Trigger>& action, std::function<void()> press, std::function<void()> release);
};
