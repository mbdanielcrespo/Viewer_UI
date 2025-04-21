#include "ui/UIElement.hpp"
#include "raylib.h"
#include "raymath.h"

UIElement::UIElement(float x, float y, float width, float height)
    : isResizing(false), isVisible(true), isActive(true)
{
    position = { x, y };
    size = { width, height };
}

void UIElement::SetPosition(float x, float y)
{
    position.x = x;
    position.y = y;
}

void UIElement::SetSize(float width, float height)
{
    size.x = width;
    size.y = height;
}

bool UIElement::IsMouseOver() const
{
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, GetBounds());
}

void UIElement::StartResizing()
{
    isResizing = true;
}

void UIElement::StopResizing()
{
    isResizing = false;
}

void UIElement::Resize(float deltaX, float deltaY)
{
    size.x += deltaX;
    size.y += deltaY;
    
    if (size.x < 50) size.x = 50;
    if (size.y < 50) size.y = 50;
}
