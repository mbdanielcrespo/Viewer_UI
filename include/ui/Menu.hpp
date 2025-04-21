#pragma once

#include "UIElement.hpp"
#include "MenuItem.hpp"
#include <string>
#include <vector>
#include <functional>

// Menu class
class Menu : public UIElement {
public:
    Menu(float x, float y, float width, float height, const std::string& title);
    virtual ~Menu();

    void Update() override;
    void Draw() override;
    
    // Menu item management
    void AddMenuItem(const std::string& label, std::function<void()> action);
    void RemoveMenuItem(const std::string& label);
    
    // Getters and setters
    const std::string& GetTitle() const { return title; }
    void SetTitle(const std::string& newTitle) { title = newTitle; }
    
private:
    std::string title;
    std::vector<MenuItem*> items;
    int selectedItem;
    bool isOpen;
    
    // UI constants
    static constexpr float TITLE_HEIGHT = 30.0f;
    static constexpr float ITEM_HEIGHT = 25.0f;
};
