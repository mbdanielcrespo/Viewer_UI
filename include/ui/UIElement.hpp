#pragma once

#include "raylib.h"

// Base UI element class
class UIElement {
public:
    UIElement(float x, float y, float width, float height);
    virtual ~UIElement() {}

    virtual void Update() = 0;
    virtual void Draw() = 0;
    
    // Getters and setters
    Rectangle GetBounds() const { return {position.x, position.y, size.x, size.y}; }
    void SetPosition(float x, float y);
    void SetSize(float width, float height);
    bool IsMouseOver() const;
    bool IsResizing() const { return isResizing; }
    
    // Resizable functionality
    void StartResizing();
    void StopResizing();
    virtual void Resize(float deltaX, float deltaY);

protected:
    Vector2 position;
    Vector2 size;
    bool isResizing;
    bool isVisible;
    bool isActive;
};
