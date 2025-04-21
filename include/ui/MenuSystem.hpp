#pragma once

#include "UIElement.hpp"
#include "NavigationBar.hpp"
#include "Viewer3D.hpp"
#include <vector>
#include <string>

// Main UI Manager class
class MenuSystem {
public:
    MenuSystem();
    ~MenuSystem();

    void Init(int screenWidth, int screenHeight);
    void Update();
    void Draw();
    void CleanUp();
    
    // UI element management
    NavigationBar* GetNavigationBar() { return navBar; }
    Menu* GetMenu(const std::string& title);
    
    // Screen management
    void SetScreenSize(int width, int height);
    
private:
    NavigationBar* navBar;
    Viewer3D* mainViewer;
    int screenWidth;
    int screenHeight;
    
    // State variables
    UIElement* resizingElement;
    Vector2 lastMousePosition;
    
    // Helper methods
    void HandleResizing();
    void SetupDefaultMenus();
};
