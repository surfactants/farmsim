#include <system/shell.hpp>

#include <util/fmouse.hpp>
#include <util/primordial.hpp>

//////////////////////////////////////////////////////////////

Shell::Shell()
    : window { sf::VideoMode::getDesktopMode(), "Fairy Farm", sf::Style::Fullscreen }
{
    sf::Cursor cursor;
    cursor.loadFromSystem(sf::Cursor::Cross);
    window.setMouseCursor(cursor);

    window.setKeyRepeatEnabled(false);

    viewGame = sf::View(sf::Vector2f(0.f, 0.f), sf::Vector2f(window.getSize()));
    viewGame.setCenter(sf::Vector2f(0.f, 0.f));

    viewUI.setSize(sf::Vector2f(window.getSize()));
    viewUI.setCenter(sf::Vector2f(window.getSize()) / 2.f);

    viewMenu.setSize(sf::Vector2f(window.getSize()));
    viewMenu.setCenter(sf::Vector2f(window.getSize()) * 0.5f);

    fpsText.setFont(Font_Manager::get(Font::MENU));
    fpsText.setString("0");
    fpsText.setFillColor(sf::Color::Red);
    fpsText.setPosition(sf::Vector2f(8.f, 8.f));

    ui.scale(window);

    menu_input.saveActions = std::bind(&Input_Handler::setRemappableActions, &input, std::placeholders::_1);

    menu_input.setActions(input.getRemappableActions());

    alignState();
}

void Shell::run()
{

    sf::Clock fpsClock;

    while (window.isOpen()) {
        input.handle();
        update();
        alignState();
        draw();
    }
}

void Shell::update()
{
    sound.update();
    music.update(state_main, state_menu, game.getState());

    frameTime = timestepClock.getElapsedTime().asSeconds();
    timestepClock.restart();
    deltaTime = frameTime / targetTime;

    switch (state_main) {
        case Main_State::MENU:
            menu->update(fMouse(window, viewUI));
            break;
        case Main_State::LOADING:
            if (loadingScreen.update()) {
                state_main = Main_State::GAME;
            }
            break;
        case Main_State::GAME:
            game.update(deltaTime);
            ui.update();
            fpsText.setString(std::to_string((int)(1.f / fpsClock.getElapsedTime().asSeconds())));
            fpsClock.restart();
            game.prepRenderer();
            break;
        default:
            break;
    }
}

void Shell::draw()
{
    window.clear();
    switch (state_main) {
        case Main_State::MENU:
            window.setView(viewUI);
            window.draw(*menu);
            break;
        case Main_State::GAME:
            window.setView(viewGame);
            window.draw(game);
            window.setView(viewUI);
            window.draw(fpsText);
            window.draw(ui);
            break;
        case Main_State::LOADING:
            window.setView(viewUI);
            window.draw(loadingScreen);
            break;
        default:
            break;
    }
    window.display();
}

void Shell::loadNewLevel()
{
    state_main = Main_State::LOADING;
    //TODO pass the game to the loader's constructor and do all this there

    std::vector<std::function<void()>> loads;
    std::vector<std::string> messages;

    //SKIPPING THE FIRST STEP ALLOWS THE RENDERING TO SWAP OVER TO LOADING BEFORE IT BEGINS
    loads.push_back(std::function<void()>([] {}));
    messages.push_back("...");

    loads.push_back(std::bind(&World::reset, &game.getWorld()));
    messages.push_back("resetting world...");

    loads.push_back(std::bind(&World::makeBiomes, &game.getWorld()));
    messages.push_back("making biomes...");

    loads.push_back(std::bind(&World::makeGrass, &game.getWorld()));
    messages.push_back("making grass...");

    loads.push_back(std::bind(&UI::init, &ui));
    messages.push_back("initializing UI.");

    loads.push_back(std::bind(&Game::startGame, &game));
    messages.push_back("finalizing world!");

    auto grab = [&]() { window.requestFocus(); };

    loads.push_back(grab);
    messages.push_back("!");

    loadingScreen.prepare(loads, messages);
}

void Shell::alignState()
{
    if (change_main) {
        change_main = false;
        switch (state_main) {
            case Main_State::QUIT:
                window.close();
                break;
            case Main_State::NEWGAME:
                loadNewLevel();
                break;
            default:
                break;
        }
    }
    if (change_menu) {
        change_menu = false;
        switch (state_menu) {
            case Menu_State::MAIN:
                menu = &menu_main;
                break;
            case Menu_State::PAUSE:
                menu = &menu_pause;
                break;
            case Menu_State::SETTINGS:
                menu = &menu_settings;
                break;
            case Menu_State::KEYS:
                menu = &menu_input;
                break;
            default:
                break;
        }
        input.menuChange();
    }
}
