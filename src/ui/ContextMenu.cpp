#include "ui/ContextMenu.hpp"
#include "raylib.h"
#include <algorithm>

ContextMenu::ContextMenu(float x, float y, float width, float height, const std::string& title)
    : UIElement(x, y, width, height), title(title), content(""), activeMenu(-1)
{
}

ContextMenu::~ContextMenu()
{
    for (auto menu : menus)
    {
        delete menu;
    }
    menus.clear();
}

void ContextMenu::Update()
{
    if (!isVisible || !isActive)
        return;
    
    Vector2 mousePos = GetMousePosition();
    
    if (isResizing)
    {
        return;
    }
    
    Rectangle rightResizeArea = {
        position.x + size.x - RESIZE_AREA_SIZE,
        position.y,
        RESIZE_AREA_SIZE,
        size.y
    };
    
    if (CheckCollisionPointRec(mousePos, rightResizeArea) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        StartResizing();
        return;
    }
    
    for (size_t i = 0; i < menus.size(); i++)
    {
        menus[i]->Update();
    }
}

void ContextMenu::Draw()
{
    if (!isVisible)
        return;
    
    DrawRectangleRec(GetBounds(), LIGHTGRAY);
    
    DrawRectangle(position.x, position.y, size.x, TITLE_HEIGHT, DARKGRAY);
    DrawText(title.c_str(), position.x + 10, position.y + 5, 20, WHITE);
    
    DrawRectangle(
        position.x + size.x - RESIZE_AREA_SIZE,
        position.y,
        RESIZE_AREA_SIZE,
        size.y,
        ColorAlpha(LIGHTGRAY, 0.5f)
    );
    
    DrawRectangle(
        position.x,
        position.y + TITLE_HEIGHT,
        size.x,
        size.y - TITLE_HEIGHT,
        RAYWHITE
    );
    
    if (!content.empty())
    {
        DrawText(
            content.c_str(),
            position.x + 10,
            position.y + TITLE_HEIGHT + 10,
            18,
            BLACK
        );
    }
    
    float menuY = position.y + TITLE_HEIGHT;
    for (size_t i = 0; i < menus.size(); i++)
    {
        menus[i]->SetPosition(position.x, menuY);
        menus[i]->Draw();
        menuY += 30;
    }
}

void ContextMenu::Resize(float deltaX, float /*deltaY*/)
{
    size.x += deltaX;
    
    if (size.x < 150) size.x = 150;
    if (size.x > 500) size.x = 500;
    
    for (auto menu : menus)
    {
        menu->SetSize(size.x, menu->GetBounds().height);
    }
}

void ContextMenu::AddMenu(const std::string& menuTitle)
{
    Menu* newMenu = new Menu(
        position.x,
        position.y + TITLE_HEIGHT + (menus.size() * 30),
        size.x,
        TITLE_HEIGHT,
        menuTitle
    );
    
    menus.push_back(newMenu);
}

Menu* ContextMenu::GetMenu(const std::string& menuTitle)
{
    auto it = std::find_if(menus.begin(), menus.end(), [&menuTitle](Menu* menu) {
        return menu->GetTitle() == menuTitle;
    });
    
    if (it != menus.end())
    {
        return *it;
    }
    
    return nullptr;
}

void ContextMenu::SetContent(const std::string& newContent)
{
    content = newContent;
}
