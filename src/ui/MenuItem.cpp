#include "ui/MenuItem.hpp"
#include "raylib.h"

MenuItem::MenuItem(const std::string& label, std::function<void()> action)
    : label(label), action(action), isHovered(false)
{
}

void MenuItem::Update()
{
    
    if (isHovered && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        if (action)
        {
            action();
        }
    }
}

void MenuItem::Draw(float x, float y, float width, bool isSelected)
{
    Rectangle itemRect = { x, y, width, 25 };
    
    Color bgColor;
    if (isSelected)
    {
        bgColor = SKYBLUE;
    }
    else if (isHovered)
    {
        bgColor = LIGHTGRAY;
    }
    else
    {
        bgColor = RAYWHITE;
    }
    
    DrawRectangleRec(itemRect, bgColor);
    
    DrawText(label.c_str(), x + 10, y + 5, 18, BLACK);
    
    DrawLine(x, y + 25, x + width, y + 25, LIGHTGRAY);
}
