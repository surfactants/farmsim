#include <ui/tooltip.hpp>

#include <resources/font_manager.hpp>

#include <util/primordial.hpp>

#include <util/vector2_stream.hpp>

Tooltip::Tooltip(std::shared_ptr<Item> item)
{
    name.setString(item->getName());
    name.setFont(Font_Manager::get(Font::UI));
    name.setFillColor(sf::Color::Black);
    name.setCharacterSize(32);

    description.setString(item->getDescription());
    description.setFont(Font_Manager::get(Font::UI));
    description.setFillColor(sf::Color::Black);
    description.setCharacterSize(24);

    int v = item->getValue();
    std::string vstring = "Value: ";
    if (v >= 0) {
        vstring += std::to_string(v);
    }
    else {
        vstring += "N/A";
    }

    value.setString(vstring);
    value.setFont(Font_Manager::get(Font::UI));
    value.setFillColor(sf::Color::Black);
    value.setCharacterSize(24);

    frame.setFillColor(sf::Color::White);
    frame.setOutlineColor(sf::Color::Black);
    frame.setOutlineThickness(1.f);
}

Tooltip::Tooltip(Reaction& reaction, std::vector<std::shared_ptr<Item>> reagants)
{
    /*
    name.setString(reaction.product);
    name.setFont(Font_Manager::get(Font::UI));
    name.setFillColor(sf::Color::Black);
    name.setCharacterSize(32);
    */

    for (auto& i : reagants) {
        sf::Sprite sprite = i->getSprite();
        std::string lstr = "x";
        for (auto& r : reaction.reagants) {
            if (equalStrings(r.name, i->getName())) {
                lstr += std::to_string(r.count);
                lstr += " " + r.name;
            }
        }
        sf::Text label;
        label.setString(lstr);
        label.setFont(Font_Manager::get(Font::UI));
        label.setFillColor(sf::Color::Black);
        label.setCharacterSize(20);

        labeled_sprites.push_back(std::make_pair(sprite, label));
    }
}

void Tooltip::setPosition(sf::Vector2f pos, sf::Vector2f window_size)
{
    sf::Vector2f pos_compute(pos);
    const static float padding = 8.f;
    frame.setPosition(pos_compute);
    pos_compute.x += padding;
    pos_compute.y += (padding / 4.f);
    name.setPosition(pos_compute);
    if (name.getString().getSize() > 0) {
        pos_compute.y += name.getLocalBounds().top + name.getLocalBounds().height + (padding / 2.f);
    }
    description.setPosition(pos_compute);
    pos_compute.y += description.getLocalBounds().top + description.getLocalBounds().height;
    value.setPosition(pos_compute);
    pos_compute.x += 24.f;
    pos_compute.y += value.getLocalBounds().top + value.getLocalBounds().height;

    float size_x;
    size_x = name.getLocalBounds().left + name.getLocalBounds().width;
    float sxh = description.getLocalBounds().left + description.getLocalBounds().width;
    if (sxh > size_x) {
        size_x = sxh;
    }
    sxh = value.getLocalBounds().left + value.getLocalBounds().width;
    if (sxh > size_x) {
        size_x = sxh;
    }

    sf::Vector2f label_offset(36.f, -8.f);

    for (auto& ls : labeled_sprites) {
        pos_compute.y += 34.f;
        ls.first.setPosition(pos_compute);
        ls.second.setPosition(pos_compute + label_offset);
        pos_compute.y += 34.f;

        sxh = pos_compute.x - frame.getPosition().x + label_offset.x
            + ls.second.getLocalBounds().left + ls.second.getLocalBounds().width;
        if (sxh > size_x) {
            size_x = sxh;
        }
    }

    pos_compute.y += padding;

    size_x += (padding * 2.f);

    sf::Vector2f frame_size(size_x, pos_compute.y - pos.y);
    frame.setSize(frame_size);

    // check for window overflow
    sf::Vector2f offset(0.f, 0.f);
    if (pos.x + frame_size.x > window_size.x) {
        offset.x = -(pos.x + frame_size.x - window_size.x);
    }
    if (pos.y + frame_size.y > window_size.y) {
        offset.y = -(pos.y + frame_size.y - window_size.y);
    }
    if (offset != sf::Vector2f(0.f, 0.f)) {
        move(offset);
    }
}

void Tooltip::move(sf::Vector2f offset)
{
    frame.move(offset);
    name.move(offset);
    description.move(offset);
    value.move(offset);
    for (auto& ls : labeled_sprites) {
        ls.first.move(offset);
        ls.second.move(offset);
    }
}

void Tooltip::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(frame, states);
    target.draw(name, states);
    target.draw(description, states);
    target.draw(value, states);
    for (const auto& ls : labeled_sprites) {
        target.draw(ls.first, states);
        target.draw(ls.second, states);
    }
}
