#pragma once

#include <string>
#include <functional>

// Menu item class
class MenuItem {
public:
    MenuItem(const std::string& label, std::function<void()> action);
    ~MenuItem() {}

    void Update();
    void Draw(float x, float y, float width, bool isSelected);
    
    // Getters
    const std::string& GetLabel() const { return label; }
    
private:
    std::string label;
    std::function<void()> action;
    bool isHovered;
};
