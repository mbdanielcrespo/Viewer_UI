#pragma once

#include "UIElement.hpp"
#include "Menu.hpp"
#include <string>
#include <vector>

// Context menu class for sidebar menus
class ContextMenu : public UIElement {
public:
    ContextMenu(float x, float y, float width, float height, const std::string& title);
    virtual ~ContextMenu();

    void Update() override;
    void Draw() override;
    void Resize(float deltaX, float deltaY) override;
    
    // Menu management
    void AddMenu(const std::string& title);
    Menu* GetMenu(const std::string& title);
    
    // Content integration
    void SetContent(const std::string& content);
    
    // Getters
    const std::string& GetTitle() const { return title; }
    
private:
    std::string title;
    std::vector<Menu*> menus;
    std::string content;
    int activeMenu;
    
    // UI constants
    static constexpr float TITLE_HEIGHT = 30.0f;
    static constexpr float RESIZE_AREA_SIZE = 10.0f;
};
