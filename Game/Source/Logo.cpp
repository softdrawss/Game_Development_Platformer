#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Logo.h"
#include "EntityManager.h"
#include "Camera.h"
#include "FadeToBlack.h"
#include "Map.h"
#include "Title.h"
#include "Defs.h"
#include "Log.h"

Logo::Logo(bool startEnabled) : Module(startEnabled)
{
	name.Create("logo");
}

// Destructor
Logo::~Logo()
{}

// Called before render is available
bool Logo::Awake(pugi::xml_node& config)
{
	LOG("Loading Logo");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Logo::Start()
{
	pugi::xml_node node = app->GetNode();
	pugi::xml_node config = node.child(name.GetString());

	logopath = (char*)config.child("pos").attribute("texturepath").as_string();
	x = config.child("pos").attribute("x").as_int();
	y = config.child("pos").attribute("y").as_int();

	img = app->tex->Load(logopath);


	app->camera->SetPosition(0, 0);
	count = 0;

	return true;
}

// Called each loop iteration
bool Logo::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Logo::Update(float dt)
{
	count++;
	if (count == 100)
	{
		app->fade->FadeBlack(this, (Module*)app->titleScreen, 90);
	}

	return true;
}

// Called each loop iteration
bool Logo::PostUpdate()
{
	app->render->DrawTexture(img, x, y, SDL_FLIP_NONE);
	
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		return false;
	}

	return true;
}

// Called before quitting
bool Logo::CleanUp()
{
	LOG("Freeing Logo");
	app->tex->UnLoad(img);
	return true;
}