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
#include "GuiManager.h"
#include "GuiControl.h"
#include "Defs.h"
#include "Log.h"

#include <string>

Debug::Debug(bool startEnabled) : Module(startEnabled) { debug = false; }
Debug::~Debug() {}

bool Debug::Start()
{
	debug = false;
	desiredFPS = 60;
	return true;
}

bool Debug::Update(float dt)
{
	OPTICK_EVENT();
	if (app->input->GetKey(SDL_SCANCODE_TAB) == KEY_DOWN)
		debug = !debug;

	if (debug)
	{
		if (app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
			variables = !variables;

		if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
			camLimits = !camLimits;

		if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
			teleport = !teleport;

		if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
			freeCam = !freeCam;

	}

	// F1/F2: Start from the first/second level
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) {
		app->fade->FadeBlack((Module*)app->scene, (Module*)app->scene); //actually this should not work like this but there is no level 2 so it works
	}

	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN) {
		//No level 2 yet :D
	}

	// F3: Start from the beginning of the current level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN) {
		app->fade->FadeBlack((Module*)app->scene, (Module*)app->scene);
	}

	// F5: Save the current game state
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) {
		app->SaveGameRequest();
	}

	// F6: Load the previous state (even across levels)
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) {
		app->LoadGameRequest();
	}

	// F7: Move between checkpoints
	if (app->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN) {

	}

	// F8:  View GUI bounds rectangles and state in different colors
	if (app->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN) {
		colourblind = !colourblind;
	}

	// F9: View colliders / logic / paths
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN) {
		drawColliders = !drawColliders;
	}

	// F10: God Mode (fly around, cannot be killed, etc)
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) {
		godMode = !godMode;
	}

	// F11: Enable/Disable FPS cap to xml value
	if (app->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) {
		controlFPS = !controlFPS;
	}

	return true;
}

bool Debug::PostUpdate()
{
	OPTICK_EVENT();
	if (debug)
		DrawDebug();

	if (drawColliders && app->physics->active)
		DrawColliders();

	return true;
}

void Debug::DrawDebug()
{
	int scale = app->win->GetScale();

	int debugX = app->render->camera.w / scale * 0.7;
	int debugY = app->render->camera.h / scale * 0.1;

	app->fonts->BlitText(debugX, debugY, 0, "variables (v)");
	//Camera Limits
	if (camLimits)
		app->fonts->BlitText(debugX, debugY + 10, 0, "camera limits (c)  on");
	else
		app->fonts->BlitText(debugX, debugY + 10, 0, "camera limits (c)  off");
	

	//Variables
	if (variables)
	{	
		//Free Camera
		if (freeCam)
			app->fonts->BlitText(debugX, debugY + 30, 0, "#free cam (f8)  on");
		else
			app->fonts->BlitText(debugX, debugY + 30, 0, "#free cam (f8)  off");
		
		//God Mode
		if (godMode)
			app->fonts->BlitText(debugX, debugY + 40, 0, "#god mode (f10)  on");
		else
			app->fonts->BlitText(debugX, debugY + 40, 0, "#god mode (f10)  off");

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

		//Player alive
		if (app->scene->player->alive)
			app->fonts->BlitText(debugX, debugY + 105, 0, "player.alive = true");
		else
			app->fonts->BlitText(debugX, debugY + 105, 0, "player.alive = false");
	}

	//Camera limits
	if (camLimits)
	{
		int scale = app->win->GetScale();

		app->scene->rectCamera.x = app->render->camera.w * 0.4;
		app->scene->rectCamera.y = app->render->camera.h * 0.4;
		app->scene->rectCamera.w = app->render->camera.w * 0.2;
		app->scene->rectCamera.h = app->render->camera.h * 0.2;
		app->render->DrawRectangle(app->scene->rectCamera, 0, 255, 0, 255, false, false);
	}

	//Teleport
	if (teleport)
	{
		if (app->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
		{
			app->scene->player->position.x = 2120;
			app->scene->player->position.y = 385;
		}
	}
}

void Debug::DrawColliders()
{
	b2World* world = app->physics->GetWorld();

	// Bonus code: this will iterate all objects in the world and draw the circles
	// You need to provide your own macro to translate meters to pixels
	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
	{
		for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
		{
			switch (f->GetType())
			{
				// Draw circles ------------------------------------------------
			case b2Shape::e_circle:
			{
				b2CircleShape* shape = (b2CircleShape*)f->GetShape();
				uint width, height;
				app->win->GetWindowSize(width, height);
				b2Vec2 pos = f->GetBody()->GetPosition();
				app->render->DrawCircle(METERS_TO_PIXELS(pos.x) * app->win->GetScale(),
					METERS_TO_PIXELS(pos.y) * app->win->GetScale(),
					METERS_TO_PIXELS(shape->m_radius) * app->win->GetScale(), 255, 255, 255);
			}
			break;

			// Draw polygons ------------------------------------------------
			case b2Shape::e_polygon:
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)f->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = b->GetWorldPoint(polygonShape->GetVertex(i));
					if (i > 0)
						app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 255, 100);

					prev = v;
				}

				v = b->GetWorldPoint(polygonShape->GetVertex(0));
				app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 255, 100, 100);
			}
			break;

			// Draw chains contour -------------------------------------------
			case b2Shape::e_chain:
			{
				b2ChainShape* shape = (b2ChainShape*)f->GetShape();
				b2Vec2 prev, v;

				for (int32 i = 0; i < shape->m_count; ++i)
				{
					v = b->GetWorldPoint(shape->m_vertices[i]);
					if (i > 0)
						app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
					prev = v;
				}

				v = b->GetWorldPoint(shape->m_vertices[0]);
				app->render->DrawLine(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y), METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y), 100, 255, 100);
			}
			break;

			// Draw a single segment(edge) ----------------------------------
			case b2Shape::e_edge:
			{
				b2EdgeShape* shape = (b2EdgeShape*)f->GetShape();
				b2Vec2 v1, v2;

				v1 = b->GetWorldPoint(shape->m_vertex0);
				v1 = b->GetWorldPoint(shape->m_vertex1);
				app->render->DrawLine(METERS_TO_PIXELS(v1.x), METERS_TO_PIXELS(v1.y), METERS_TO_PIXELS(v2.x), METERS_TO_PIXELS(v2.y), 100, 100, 255);
			}
			break;
			}

			// TODO 1: If mouse button 1 is pressed ...
			// app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN
			// test if the current body contains mouse position
		}
	}
}