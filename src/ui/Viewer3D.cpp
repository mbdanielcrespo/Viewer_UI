#include "ui/Viewer3D.hpp"
#include "raylib.h"
#include "raymath.h"

Viewer3D::Viewer3D(float x, float y, float width, float height)
    : UIElement(x, y, width, height), isDragging(false), isRotating(false), isPanning(false),
      isDarkTheme(true), showGrid(true), gridSize(50), gridSpacing(1.0f)
{
    cameraPosition = { 0.0f, 5.0f, 10.0f };
    targetPosition = { 0.0f, 0.0f, 0.0f };
    camera.position = cameraPosition;
    camera.target = targetPosition;
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    
    previousMousePosition = { 0.0f, 0.0f };
}

Viewer3D::~Viewer3D()
{
}

void Viewer3D::Update()
{
    if (!isVisible || !isActive)
        return;
    
    HandleMouseControls();
    UpdateCamera();
}

void Viewer3D::Draw()
{
    if (!isVisible)
        return;
    
    Rectangle bounds = GetBounds();
    BeginScissorMode(bounds.x, bounds.y, bounds.width, bounds.height);
    
    DrawRectangleLinesEx(bounds, 1.0f, LIGHTGRAY);
    
    Color bgColor = isDarkTheme ? DARKGRAY : RAYWHITE;
    DrawRectangle(bounds.x, bounds.y, bounds.width, bounds.height, bgColor);
    
    BeginMode3D(camera);
    
    if (showGrid)
    {
        DrawGrid();
    }
    
    DrawLine3D({ 0.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, 0.0f }, RED);
    DrawLine3D({ 0.0f, 0.0f, 0.0f }, { 0.0f, 5.0f, 0.0f }, GREEN);
    DrawLine3D({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 5.0f }, BLUE);
    
    EndMode3D();
    
    Color textColor = isDarkTheme ? RAYWHITE : BLACK;
    DrawText(TextFormat("Camera: %.1f, %.1f, %.1f", camera.position.x, camera.position.y, camera.position.z),
        bounds.x + 10, bounds.y + 10, 18, textColor);
    
    DrawText("Controls: Right-click = Orbit | Middle-click = Pan | Wheel = Zoom | Shift+Right-click = Pivot",
        bounds.x + 10, bounds.y + bounds.height - 30, 10, textColor);
    
    EndScissorMode();
}

void Viewer3D::SetupCamera()
{
    cameraPosition = { 0.0f, 5.0f, 10.0f };
    camera.position = cameraPosition;
    camera.target = { 0.0f, 0.0f, 0.0f };
}

void Viewer3D::UpdateCamera()
{
    camera.position = cameraPosition;
}

void Viewer3D::DrawGrid()
{
    float halfSize = gridSize * gridSpacing / 2.0f;
    Color gridColor = isDarkTheme ? LIGHTGRAY : GRAY;
    Color mainAxisColor = isDarkTheme ? WHITE : BLACK;
    
    for (int i = -gridSize; i <= gridSize; i++)
    {
        float position = i * gridSpacing;
        
        DrawLine3D(
            { -halfSize, 0.0f, position }, 
            { halfSize, 0.0f, position }, 
            (i == 0) ? GREEN : ColorAlpha(gridColor, 0.8f)
        );
        
        DrawLine3D(
            { position, 0.0f, -halfSize }, 
            { position, 0.0f, halfSize }, 
            (i == 0) ? RED : ColorAlpha(gridColor, 0.8f)
        );
    }
}

void Viewer3D::HandleMouseControls()
{
    Vector2 mousePos = GetMousePosition();
    
    if (IsMouseOver())
    {
        if (!isDragging && !isRotating && !isPanning)
        {
            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && !IsKeyDown(KEY_LEFT_SHIFT))
            {
                isRotating = true;
                previousMousePosition = mousePos;
            }
            
            else if (IsMouseButtonPressed(MOUSE_MIDDLE_BUTTON))
            {
                isPanning = true;
                previousMousePosition = mousePos;
            }
            
            else if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON) && IsKeyDown(KEY_LEFT_SHIFT))
            {
                isDragging = true;
                previousMousePosition = mousePos;
            }
        }
        
        if (isRotating)
        {
            if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
            {
                Vector2 mouseDelta = Vector2Subtract(mousePos, previousMousePosition);
                
                float angleY = -mouseDelta.x * 0.01f;
                float angleX = mouseDelta.y * 0.01f;
                
                Vector3 toTarget = Vector3Subtract(cameraPosition, targetPosition);
                
                float distance = Vector3Length(toTarget);
                
                float theta = atan2f(toTarget.x, toTarget.z);
                float phi = atan2f(toTarget.y, sqrtf(toTarget.x*toTarget.x + toTarget.z*toTarget.z));
                
                theta += angleY;
                phi = Clamp(phi + angleX, -PI/2.1f, PI/2.1f);
                
                cameraPosition.x = targetPosition.x + distance * sinf(theta) * cosf(phi);
                cameraPosition.y = targetPosition.y + distance * sinf(phi);
                cameraPosition.z = targetPosition.z + distance * cosf(theta) * cosf(phi);
                
                camera.position = cameraPosition;
                
                previousMousePosition = mousePos;
            }
            else
            {
                isRotating = false;
            }
        }
        
        else if (isPanning)
        {
            if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON))
            {
                Vector2 mouseDelta = Vector2Subtract(mousePos, previousMousePosition);
                
                Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
                Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));
                Vector3 up = Vector3CrossProduct(right, forward);
                
                Vector3 panRight = Vector3Scale(right, -mouseDelta.x * 0.05f);
                Vector3 panUp = Vector3Scale(up, mouseDelta.y * 0.05f);
                Vector3 totalPan = Vector3Add(panRight, panUp);
                
                cameraPosition = Vector3Add(cameraPosition, totalPan);
                targetPosition = Vector3Add(targetPosition, totalPan);
                
                camera.position = cameraPosition;
                camera.target = targetPosition;
                
                previousMousePosition = mousePos;
            }
            else
            {
                isPanning = false;
            }
        }
        
        else if (isDragging)
        {
            if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON) && IsKeyDown(KEY_LEFT_SHIFT))
            {
                Vector2 mouseDelta = Vector2Subtract(mousePos, previousMousePosition);
                
                Ray ray = GetMouseRay(mousePos, camera);
                
                float t = -ray.position.y / ray.direction.y;
                
                if (t > 0)
                {
                    Vector3 intersection = Vector3Add(
                        ray.position, 
                        Vector3Scale(ray.direction, t)
                    );
                    
                    targetPosition = intersection;
                    camera.target = targetPosition;
                }
                
                previousMousePosition = mousePos;
            }
            else
            {
                isDragging = false;
            }
        }
        
        float wheelMove = GetMouseWheelMove();
        if (wheelMove != 0)
        {
            Vector3 direction = Vector3Subtract(targetPosition, cameraPosition);
            direction = Vector3Normalize(direction);
            
            float zoomAmount = wheelMove * 1.0f;
            cameraPosition = Vector3Add(cameraPosition, Vector3Scale(direction, zoomAmount));
            
            Vector3 distVec = Vector3Subtract(targetPosition, cameraPosition);
            float distance = Vector3Length(distVec);
            
            if (distance < 1.0f)
            {
                direction = Vector3Normalize(distVec);
                cameraPosition = Vector3Subtract(targetPosition, Vector3Scale(direction, 1.0f));
            }
            
            camera.position = cameraPosition;
        }
    }
    else
    {
        isDragging = false;
        isRotating = false;
        isPanning = false;
    }
}
