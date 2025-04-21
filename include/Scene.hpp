#pragma once

#include "ui/MenuSystem.hpp"

class Scene {
public:
    Scene();
    ~Scene();

    void Init();
    
    void Update();

    void Draw();

    void CleanUp();

private:
    MenuSystem* menuSystem;
};
