#include <ui/inventory_interface.hpp>

#include <SFML/Window/Mouse.hpp>

#include <resources/font_manager.hpp>

const float Inventory_Interface::cell_padding = Inventory_Cell::frameOutlineSize;

Inventory_Interface::Inventory_Interface(Player_Inventory& inventory)
    : inventory { inventory }
{
    readInventory();

    setEquippedIndex(0);

    dragCountText.setFont(Font_Manager::get(Font::UI));
    dragCountText.setFillColor(sf::Color::Black);
}

void Inventory_Interface::readInventory()
{
    size_t nr = cells.size();
    size_t nc = cells[0].size();

    if (nr != inventory.rowCount
     || nc != inventory.rowWidth) {
        resize();
    }

    if (nc != inventory.rowWidth);

    for (size_t r = 0; r < nr; r++) {
        for (size_t c = 0; c < nc; c++) {
            cells[r][c].setItem(inventory(r, c));
        }
    }
}

void Inventory_Interface::resize()
{
    cells.resize(inventory.rowCount);

    for (auto& row : cells) {
        row.resize(inventory.rowWidth);
    }

    placeCells();
}

void Inventory_Interface::placeCells()
{
    float x = 96.f;
    sf::Vector2f pos(x, screenHeight - x);
    size_t nr = cells.size();
    size_t nc = cells[0].size();
    for (size_t r = 0; r < nr; r++) {
        for (size_t c = 0; c < nc; c++) {
            cells[r][c].setPosition(pos);
            pos.x += Inventory_Cell::frameSize.x + cell_padding;
        }
        pos.x = x;
        if (r == 0) {
            pos.y -= Inventory_Cell::frameSize.y * 5.f;
        }
        else {
            pos.y += Inventory_Cell::frameSize.y + cell_padding;
        }
    }
}

void Inventory_Interface::pollChanges()
{
    if (inventory.changed) {
        readInventory();
        inventory.changed = false;
    }
}

void Inventory_Interface::setEquippedIndex(size_t active)
{
    if (active >= SIZE_MAX) {
        active = cells[0].size() - 1;
    }
    if (active >= cells[0].size()) {
        active = 0;
    }

    cells[0][equippedIndex].deactivate();
    equippedIndex = active;
    cells[0][equippedIndex].activate();
}

size_t Inventory_Interface::getEquippedIndex()
{
    return equippedIndex;
}

void Inventory_Interface::scrollLeft()
{
    if (equippedIndex == 0) {
        setEquippedIndex(cells.size() - 1);
    }
    else {
        setEquippedIndex(equippedIndex - 1);
    }
}

void Inventory_Interface::scrollRight()
{
    setEquippedIndex(equippedIndex + 1);
}

void Inventory_Interface::close()
{
    if (dragging) {
        endDrag();
    }
    expanded = false;
}

void Inventory_Interface::startDrag()
{
    const sf::Vector2i mpos = sf::Mouse::getPosition();
    sf::Vector2i index = mousedIndex(mpos);
    if (index.x >= 0 && index.y >= 0) {
        dragItem = std::make_shared<Item>(*cells[index.x][index.y].getItem());
        if (dragItem) {
            inventory.clearItem(index.x, index.y);
            cells[index.x][index.y].clearItem();
            dragging = true;
            dragStartIndex = index;
            std::string text = "";
            if (dragItem->count() > 1) {
                text = std::to_string(dragItem->count());
            }
            dragCountText.setString(text);
        }
    }
}

void Inventory_Interface::checkDrag()
{
    if (dragging && dragItem) {
        sf::Vector2f pos(sf::Mouse::getPosition());
        dragItem->setPosition(pos);

        pos.x -= Inventory_Cell::frameSize.x * .4f;
        dragCountText.setPosition(pos);
    }
}

void Inventory_Interface::endDrag()
{
    const sf::Vector2i mpos = sf::Mouse::getPosition();
    sf::Vector2i index = mousedIndex(mpos);
    if (index.x >= 0 && index.y >= 0) {
        if (cells[index.x][index.y].getItem()) {
            cells[dragStartIndex.x][dragStartIndex.y].setItem(cells[index.x][index.y].getItem());
            cells[index.x][index.y].clearItem();
        }
        cells[index.x][index.y].setItem(dragItem.get());
    }
    else {
        cells[dragStartIndex.x][dragStartIndex.y].setItem(dragItem.get());
    }
    sf::Vector2i i = dragStartIndex;
    if (cells[i.x][i.y].getItem()) {
        inventory.placeItem(i.x, i.y, cells[i.x][i.y].getItem());
    }
    i = index;
    if (i.x >= 0 && i.y >= 0 && cells[i.x][i.y].getItem()) {
        inventory.placeItem(i.x, i.y, cells[i.x][i.y].getItem());
    }
    dragging = false;
    dragItem = nullptr;
}

sf::Vector2i Inventory_Interface::mousedIndex(const sf::Vector2i& mpos)
{
    sf::Vector2i index(-1, -1);

    size_t nr = cells.size();
    size_t nc = cells[0].size();

    for (size_t r = 0; r < nr; r++) {
        for (size_t c = 0; c < nc; c++) {
            if (cells[r][c].contains(mpos)) {
                index = sf::Vector2i(r, c);
                goto END;
            }
        }
    }

    END: return index;
}

void Inventory_Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    size_t nr = cells.size();
    size_t nc = cells[0].size();
    for (size_t r = 0; r < nr; r++) {
        for (size_t c = 0; (c < nc) && (expanded || r == 0); c++) {
            target.draw(cells[r][c], states);
        }
    }

    if (dragging && dragItem) {
        target.draw(*dragItem, states);
        target.draw(dragCountText, states);
    }
}