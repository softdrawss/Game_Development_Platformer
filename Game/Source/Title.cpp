#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Title.h"
#include "EntityManager.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"
#include "Player.h"

#include "Scene.h"

#include "Debug.h"


Title::Title(bool startEnabled) : Module(startEnabled)
{
	name.Create("Title");
	active = false;
}

// Destructor
Title::~Title()
{}

// Called before render is available
bool Title::Awake(pugi::xml_node& config)
{
	LOG("Loading Title");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Title::Start()
{
	img = app->tex->Load("Assets/Textures/menu.png");

	return true;
}

// Called each loop iteration
bool Title::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Title::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
  		app->fade->FadeBlack(this, (Module*)app->scene, 90);
	}

	return true;
}

// Called each loop iteration
bool Title::PostUpdate()
{
	app->render->DrawTexture(img, 0, 0);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		return false;
	}

	return true;
}

// Called before quitting
bool Title::CleanUp()
{
	LOG("Freeing title");
	app->tex->UnLoad(img);
	return true;
}