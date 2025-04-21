#pragma once

#include "UIElement.hpp"
#include "raylib.h"

// 3D Viewer class
class Viewer3D : public UIElement {
public:
    Viewer3D(float x, float y, float width, float height);
    virtual ~Viewer3D();

    void Update() override;
    void Draw() override;
    
    // Camera control
    void SetupCamera();
    void UpdateCamera();
    void SetDarkTheme(bool darkMode) { isDarkTheme = darkMode; }
    bool IsDarkTheme() const { return isDarkTheme; }
    
private:
    Camera3D camera;
    Vector3 cameraPosition;
    Vector3 targetPosition;
    bool isDragging;
    bool isRotating;
    bool isPanning;
    bool isDarkTheme;
    Vector2 previousMousePosition;
    
    // Grid parameters
    bool showGrid;
    int gridSize;
    float gridSpacing;
    
    // Helper methods
    void DrawGrid();
    void HandleMouseControls();
};
