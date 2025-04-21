#include "raylib.h"
#include "Scene.hpp"

int main(void)
{
	const int screenWidth = 1024;
	const int screenHeight = 768;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(screenWidth, screenHeight, "Hoist Management System");

	SetTargetFPS(60);
	
	Scene scene;
	scene.Init();

	while (!WindowShouldClose())
	{
		scene.Update();

		BeginDrawing();
			scene.Draw();
		EndDrawing();
	}

	scene.CleanUp();
	CloseWindow();
	
	return 0;
}
