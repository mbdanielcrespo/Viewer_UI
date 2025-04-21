#include "Scene.hpp"
#include "raylib.h"

Scene::Scene() : menuSystem(nullptr) {}
Scene::~Scene() { CleanUp(); }

void Scene::Init() 
{
	menuSystem = new MenuSystem();
	menuSystem->Init(GetScreenWidth(), GetScreenHeight());
}

void Scene::Update() 
{
	if (IsWindowResized())
		menuSystem->SetScreenSize(GetScreenWidth(), GetScreenHeight());
	menuSystem->Update();
}

void Scene::Draw() { menuSystem->Draw(); }

void Scene::CleanUp() 
{
	if (menuSystem)
	{
		delete menuSystem;
		menuSystem = nullptr;
	}
}
