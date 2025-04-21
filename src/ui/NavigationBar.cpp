#include "ui/NavigationBar.hpp"
#include "raylib.h"
#include <algorithm>

NavigationBar::NavigationBar(float x, float y, float width, float height)
    : UIElement(x, y, width, height), activeMenu(-1), hoveredMenu(-1)
{
}

NavigationBar::~NavigationBar()
{
    for (auto menu : menus)
    {
        delete menu;
    }
    menus.clear();
}

void NavigationBar::Update()
{
    if (!isVisible || !isActive)
        return;
    
    Vector2 mousePos = GetMousePosition();
    bool mouseOverMenu = false;
    
    hoveredMenu = -1;
    for (size_t i = 0; i < menus.size(); i++)
    {
        float menuX = position.x + (i * (100.0f + MENU_SPACING));
        Rectangle menuRect = { menuX, position.y, 100.0f, MENU_HEIGHT };
        
        if (CheckCollisionPointRec(mousePos, menuRect))
        {
            hoveredMenu = static_cast<int>(i);
            mouseOverMenu = true;
            
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
            {
                if (activeMenu == static_cast<int>(i))
                {
                    activeMenu = -1;
                }
                else
                {
                    activeMenu = static_cast<int>(i);
                }
            }
            break;
        }
    }
    
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && !mouseOverMenu && activeMenu != -1)
    {
        Menu* activeMenuPtr = menus[activeMenu];
        if (!activeMenuPtr->IsMouseOver())
        {
            activeMenu = -1;
        }
    }
    
    if (activeMenu != -1 && activeMenu < static_cast<int>(menus.size()))
    {
        Menu* activeMenuPtr = menus[activeMenu];
        
        float menuX = position.x + (activeMenu * (100.0f + MENU_SPACING));
        activeMenuPtr->SetPosition(menuX, position.y + MENU_HEIGHT);
        
        activeMenuPtr->Update();
    }
}

void NavigationBar::Draw()
{
    if (!isVisible)
        return;
    
    DrawRectangleRec(GetBounds(), DARKGRAY);
    
    for (size_t i = 0; i < menus.size(); i++)
    {
        float menuX = position.x + (i * (100.0f + MENU_SPACING));
        Rectangle menuRect = { menuX, position.y, 100.0f, MENU_HEIGHT };
        
        Color bgColor;
        if (static_cast<int>(i) == activeMenu)
        {
            bgColor = BLUE;
        }
        else if (static_cast<int>(i) == hoveredMenu)
        {
            bgColor = GRAY;
        }
        else
        {
            bgColor = DARKGRAY;
        }
        
        DrawRectangleRec(menuRect, bgColor);
        DrawRectangleLinesEx(menuRect, 1.0f, BLACK);
        
        DrawText(menus[i]->GetTitle().c_str(), menuX + 10, position.y + 5, 18, WHITE);
    }
    
    if (activeMenu != -1 && activeMenu < static_cast<int>(menus.size()))
    {
        menus[activeMenu]->Draw();
    }
}

void NavigationBar::Resize(float deltaX, float /*deltaY*/)
{
    size.x += deltaX;
    
    if (size.x < 200) size.x = 200;
}

void NavigationBar::AddMenu(const std::string& title)
{
    float menuX = position.x + (menus.size() * (100.0f + MENU_SPACING));
    Menu* newMenu = new Menu(menuX, position.y + MENU_HEIGHT, 150.0f, 300.0f, title);
    menus.push_back(newMenu);
}

Menu* NavigationBar::GetMenu(const std::string& title)
{
    auto it = std::find_if(menus.begin(), menus.end(), [&title](Menu* menu) {
        return menu->GetTitle() == title;
    });
    
    if (it != menus.end())
    {
        return *it;
    }
    
    return nullptr;
}
