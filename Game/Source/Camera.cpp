#include "App.h"
#include "Camera.h"
#include "Render.h"
#include "Physics.h"
#include "Map.h"
#include "Debug.h"
#include "Scene.h"
#include "Window.h"

#include "Defs.h"
#include "Log.h"

#include <math.h>


Camera::Camera(bool startEnabled) : Module(startEnabled) {}

Camera::~Camera() {}

// Called before render is available
bool Camera::Awake(pugi::xml_node& config)
{
    LOG("Setting camera");
    bool ret = true;

    
    return ret;
}

bool Camera::Start()
{
	camSpeed = 6 * app->win->GetScale();
	
	if (DEBUG_MODE) { app->camera->cameraMode = PLAYER; }
	else { cameraMode = CINEMATIC; }

	SetPosition(0, 0);

    return true;
}

bool Camera::Update(float dt)
{
    return true;
}

bool Camera::PostUpdate()
{
	if (app->debug->freeCam)
	{
		FreeCamera();
	}
	else if (cameraMode == PLAYER)
	{
		FollowPlayer();
		CameraLimits();
	}
	else if (cameraMode == CINEMATIC)
	{

	}
	
    return true;
}

bool Camera::CleanUp()
{
    LOG("Unloading camera?");

    return true;
}

void Camera::SetPosition(int x, int y)
{
    app->render->camera.x = x;
    app->render->camera.y = y;
}

void Camera::AddPosition(int x, int y)
{
    app->render->camera.x += x;
    app->render->camera.y += y;
}

void Camera::FollowPlayer()
{
	int scale = app->win->GetScale();
	int player_X = app->scene->player->position.x;
	int player_Y = app->scene->player->position.y;

	//Camera behaviour
	//Left
	//if (player_X * scale + app->render->camera.x < app->render->camera.w * 0.4)
	//	app->render->camera.x = -player_X * scale + (app->render->camera.w * 0.4);
	////right
	//else if (player_X * scale + app->render->camera.x > app->render->camera.w * 0.6)
	//	app->render->camera.x = -player_X * scale + (app->render->camera.w * 0.6);
	////up
	//if (player_Y * scale + app->render->camera.y < app->render->camera.h * 0.4)
	//	app->render->camera.y = -player_Y * scale + (app->render->camera.h * 0.4);
	////down
	//else if (player_Y * scale + app->render->camera.y > app->render->camera.h * 0.6)
	//	app->render->camera.y = -player_Y * scale + (app->render->camera.h * 0.6);


	//Camera behaviour_Smooth (1472 - 832)
	//Left
	if (player_X * scale + app->render->camera.x < 588)
		app->render->camera.x = -player_X * scale + (588);
	//right
	else if (player_X * scale + app->render->camera.x > 883)
		app->render->camera.x = -player_X * scale + (883);
	//up
	if (player_Y * scale + app->render->camera.y < 332)
		app->render->camera.y = -player_Y * scale + (332);
	//down
	else if (player_Y * scale + app->render->camera.y > 499)
		app->render->camera.y = -player_Y * scale + (499);
}

void Camera::CameraLimits()
{
	if (app->render->camera.x > 0)
		app->render->camera.x = 0;
	else if (app->render->camera.x < -161)
		app->render->camera.x = -161;
	if (app->render->camera.y > 0)
		app->render->camera.y = 0;
	else if (app->render->camera.y < -1560)
		app->render->camera.y = -1560;

	//162, 1560
}

void Camera::FreeCamera()
{
	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += camSpeed;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= camSpeed;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += camSpeed;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= camSpeed;

	//Map limits
	if (app->render->camera.x > 0)
		app->render->camera.x = 0;
	else if (app->render->camera.x > app->map->mapData.width * app->map->mapData.tileWidth)
	{
		app->render->camera.x = app->map->mapData.width * app->map->mapData.tileWidth - app->render->camera.w;
	}
	if (app->render->camera.y > 0)
	{

	}
	else if (app->render->camera.y > app->map->mapData.height * app->map->mapData.tileHeight)
	{
		app->render->camera.y = app->map->mapData.width * app->map->mapData.tileWidth - app->render->camera.h;
	}
}