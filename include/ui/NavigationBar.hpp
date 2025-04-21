#pragma once

#include "UIElement.hpp"
#include "Menu.hpp"
#include <vector>
#include <string>

// Navigation bar class for top menu
class NavigationBar : public UIElement {
public:
    NavigationBar(float x, float y, float width, float height);
    virtual ~NavigationBar();

    void Update() override;
    void Draw() override;
    void Resize(float deltaX, float deltaY) override;
    
    // Menu management
    void AddMenu(const std::string& title);
    Menu* GetMenu(const std::string& title);
    
private:
    std::vector<Menu*> menus;
    int activeMenu;
    int hoveredMenu;
    
    // UI constants
    static constexpr float MENU_SPACING = 10.0f;
    static constexpr float MENU_HEIGHT = 25.0f;
};
