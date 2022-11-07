#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Fonts.h"
#include "EntityManager.h"
#include "Map.h"
#include "Physics.h"
#include "FadeToBlack.h"
#include "Debug.h"
#include "Logo.h"
#include "Title.h"
#include "Scene.h"
#include "Ending.h"

#include "Defs.h"
#include "Log.h"

#include <string>

Debug::Debug(bool startEnabled) : Module(startEnabled) { debug = false; }
Debug::~Debug() {}

bool Debug::Start()
{
	debug = false;
	return true;
}

bool Debug::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
		debug = !debug;

	if (debug)
	{
		if (app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
			variables = !variables;

		if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
			camLimits = !camLimits;
	}

	// F1/F2: Start from the first/second level
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {

	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {

	}

	// F3: Start from the beginning of the current level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {

	}

	// F5: Save the current game state
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		app->SaveGameRequest();
	}

	// F6: Load the previous state (even across levels)
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		app->LoadGameRequest();
	}

	// F9: View colliders / logic
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		app->physics->debug = !app->physics->debug;
	}

	// F10: God Mode (fly around, cannot be killed, etc)
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godMode = !godMode;
	}

	// F11: Free camera
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		freeCam = !freeCam;
	}
	return true;
}

bool Debug::PostUpdate()
{
	if (debug)
		DrawDebug();

	return true;
}

void Debug::DrawDebug()
{
	int scale = app->win->GetScale();

	int debugX = app->render->camera.w / scale * 0.7;
	int debugY = app->render->camera.h / scale * 0.1;

	app->fonts->BlitText(debugX, debugY, 0, "v - variables");
	app->fonts->BlitText(debugX, debugY + 10, 0, "c - camera limits");

	//Variables
	if (variables)
	{	
		//God Mode
		if (!godMode)
			app->fonts->BlitText(debugX, debugY + 30, 0, "#god mode (f10)  off");
		else
			app->fonts->BlitText(debugX, debugY + 30, 0, "#god mode (f10)  on");
		
		//Free Camera
		if (!freeCam)
			app->fonts->BlitText(debugX, debugY + 40, 0, "#free cam (f11)  off");
		else
			app->fonts->BlitText(debugX, debugY + 40, 0, "#free cam (f11)  on");

		//Player x, y
		app->fonts->BlitText(debugX , debugY + 55, 0, "player.x =");
		app->fonts->BlitText(debugX + 88, debugY + 55, 0, std::to_string(app->scene->player->position.x).c_str());

		app->fonts->BlitText(debugX, debugY + 65, 0, "player.y =");
		app->fonts->BlitText(debugX + 88, debugY + 65, 0, std::to_string(app->scene->player->position.y).c_str());
		//Camera x, y
		app->fonts->BlitText(debugX, debugY + 80, 0, "camera.x =");
		app->fonts->BlitText(debugX + 88, debugY + 80, 0, std::to_string(app->render->camera.x).c_str());

		app->fonts->BlitText(debugX, debugY + 90, 0, "camera.y =");
		app->fonts->BlitText(debugX + 88, debugY + 90, 0, std::to_string(app->render->camera.y).c_str());
		//Camera w, h
		app->fonts->BlitText(debugX, debugY + 105, 0, "debug.x =");
		app->fonts->BlitText(debugX + 88, debugY + 105, 0, std::to_string(debugX).c_str());

		app->fonts->BlitText(debugX, debugY + 115, 0, "debug.y =");
		app->fonts->BlitText(debugX + 88, debugY + 115, 0, std::to_string(debugY).c_str());
	}

	//Camera limits
	if (camLimits)
	{
		int scale = app->win->GetScale();

		app->scene->rectCamera.x = -app->render->camera.x + (app->render->camera.w * 0.4);
		app->scene->rectCamera.y = -app->render->camera.y + (app->render->camera.h * 0.4);
		app->scene->rectCamera.w = app->render->camera.w * 0.2;
		app->scene->rectCamera.h = app->render->camera.h * 0.2;
		app->render->DrawRectangle(app->scene->rectCamera, 0, 255, 0, 255, false, false);
	}

	//Teleport
	/*if (teleport)
	{
		app->fonts->BlitText(10, spawnBox + 40, 0, "TELEPORT.");
		app->fonts->BlitText(75, spawnBox + 40, 0, "1.START");
		app->fonts->BlitText(75, spawnBox + 50, 0, "2.FENCE");
		app->fonts->BlitText(75, spawnBox + 60, 0, "3.BRIDGE");
		app->fonts->BlitText(75, spawnBox + 70, 0, "4.BOSS");

		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN) {
			app->scene->player->position.x = 230;
			app->scene->player->position.y = -60;
			app->render->camera.x = app->scene->player->position.x - 100;
			app->render->camera.y = app->scene->player->position.x - 200;
		}
	}*/
}