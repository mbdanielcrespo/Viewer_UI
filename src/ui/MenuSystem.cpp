#include "ui/MenuSystem.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>

MenuSystem::MenuSystem() 
    : navBar(nullptr), mainViewer(nullptr), screenWidth(800), screenHeight(450), resizingElement(nullptr)
{
    lastMousePosition = { 0, 0 };
}

MenuSystem::~MenuSystem()
{
    CleanUp();
}

void MenuSystem::Init(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    
    const int NAV_BAR_HEIGHT = 30;
    navBar = new NavigationBar(0, 0, screenWidth, NAV_BAR_HEIGHT);
    
    mainViewer = new Viewer3D(0, NAV_BAR_HEIGHT, screenWidth, screenHeight - NAV_BAR_HEIGHT);
    
    mainViewer->SetDarkTheme(true);
    
    SetupDefaultMenus();
}

void MenuSystem::SetupDefaultMenus()
{
    navBar->AddMenu("File");
    navBar->AddMenu("Edit");
    navBar->AddMenu("View");
    navBar->AddMenu("Help");
    
    Menu* fileMenu = navBar->GetMenu("File");
    if (fileMenu)
    {
        fileMenu->AddMenuItem("New", []()   {});
        fileMenu->AddMenuItem("Open", []()  {});
        fileMenu->AddMenuItem("Save", []()  {});
        fileMenu->AddMenuItem("Exit", []()  {});
    }
    
    Menu* editMenu = navBar->GetMenu("Edit");
    if (editMenu)
    {
        editMenu->AddMenuItem("Undo", []()  {});
        editMenu->AddMenuItem("Redo", []()  {});
        editMenu->AddMenuItem("Cut", []()   {});
        editMenu->AddMenuItem("Copy", []()  {});
        editMenu->AddMenuItem("Paste", []() {});
    }
    
    Menu* viewMenu = navBar->GetMenu("View");
    if (viewMenu)
    {
        viewMenu->AddMenuItem("Grid", []()          {});
        viewMenu->AddMenuItem("Wireframe", []()     {});
        viewMenu->AddMenuItem("Toggle Theme", []()  {});
    }
    
    Menu* helpMenu = navBar->GetMenu("Help");
    if (helpMenu)
    {
        helpMenu->AddMenuItem("Documentation", []() {});
        helpMenu->AddMenuItem("About", []()         {});
    }
}

void MenuSystem::Update()
{
    HandleResizing();
    
    if (navBar)
    {
        navBar->Update();
        
        if (navBar->IsResizing() && !resizingElement)
        {
            resizingElement = navBar;
            lastMousePosition = GetMousePosition();
        }
    }
    
    if (mainViewer)
    {
        mainViewer->Update();
    }
}

void MenuSystem::Draw()
{
    ClearBackground(DARKGRAY);
    
    if (mainViewer)
    {
        mainViewer->Draw();
    }
    
    if (navBar)
    {
        navBar->Draw();
    }
}

void MenuSystem::CleanUp()
{
    if (navBar)
    {
        delete navBar;
        navBar = nullptr;
    }
    
    if (mainViewer)
    {
        delete mainViewer;
        mainViewer = nullptr;
    }
    
    resizingElement = nullptr;
}

Menu* MenuSystem::GetMenu(const std::string& title)
{
    if (navBar)
        return navBar->GetMenu(title);
    return nullptr;
}

void MenuSystem::SetScreenSize(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    
    if (navBar)
        navBar->SetSize(screenWidth, navBar->GetBounds().height);
    
    if (mainViewer && navBar)
    {
        float navBarHeight = navBar->GetBounds().height;
        mainViewer->SetPosition(0, navBarHeight);
        mainViewer->SetSize(screenWidth, screenHeight - navBarHeight);
    }
    else if (mainViewer)
    {
        mainViewer->SetPosition(0, 0);
        mainViewer->SetSize(screenWidth, screenHeight);
    }
}

void MenuSystem::HandleResizing()
{
    if (resizingElement && IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        Vector2 currentMousePosition = GetMousePosition();
        Vector2 mouseDelta = Vector2Subtract(currentMousePosition, lastMousePosition);
        
        resizingElement->Resize(mouseDelta.x, mouseDelta.y);
        
        lastMousePosition = currentMousePosition;
    }
    else if (resizingElement && IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        resizingElement->StopResizing();
        resizingElement = nullptr;
    }
}
