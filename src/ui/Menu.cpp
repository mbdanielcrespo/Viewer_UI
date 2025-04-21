#include "ui/Menu.hpp"
#include "raylib.h"
#include <algorithm>

Menu::Menu(float x, float y, float width, float height, const std::string& title)
    : UIElement(x, y, width, height), title(title), selectedItem(-1), isOpen(false)
{
}

Menu::~Menu()
{
    for (auto item : items)
    {
        delete item;
    }
    items.clear();
}

void Menu::Update()
{
    if (!isVisible || !isActive)
        return;
    
    Rectangle titleBar = { position.x, position.y, size.x, TITLE_HEIGHT };
    Vector2 mousePos = GetMousePosition();
    
    if (CheckCollisionPointRec(mousePos, titleBar) && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        isOpen = !isOpen;
    }
    
    if (isOpen)
    {
        selectedItem = -1;
        
        for (size_t i = 0; i < items.size(); i++)
        {
            Rectangle itemRect = {
                position.x,
                position.y + TITLE_HEIGHT + (i * ITEM_HEIGHT),
                size.x,
                ITEM_HEIGHT
            };
            
            if (CheckCollisionPointRec(mousePos, itemRect))
            {
                selectedItem = i;
                break;
            }
        }
        
        if (selectedItem >= 0 && static_cast<size_t>(selectedItem) < items.size())
        {
            items[selectedItem]->Update();
        }
    }
}

void Menu::Draw()
{
    if (!isVisible)
        return;
    
    DrawRectangle(position.x, position.y, size.x, TITLE_HEIGHT, DARKGRAY);
    DrawText(title.c_str(), position.x + 10, position.y + 5, 20, WHITE);
    
    if (isOpen)
    {
        for (size_t i = 0; i < items.size(); i++)
        {
            items[i]->Draw(
                position.x,
                position.y + TITLE_HEIGHT + (i * ITEM_HEIGHT),
                size.x,
                selectedItem == static_cast<int>(i)
            );
        }
    }
}

void Menu::AddMenuItem(const std::string& label, std::function<void()> action)
{
    items.push_back(new MenuItem(label, action));
}

void Menu::RemoveMenuItem(const std::string& label)
{
    auto it = std::find_if(items.begin(), items.end(), [&label](MenuItem* item) {
        return item->GetLabel() == label;
    });
    
    if (it != items.end())
    {
        delete *it;
        items.erase(it);
    }
}
