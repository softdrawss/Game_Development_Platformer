#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Logo.h"
#include "EntityManager.h"
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
	img = app->tex->Load("Assets/Textures/logo.png");
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
	app->render->DrawTexture(img, 0, 0);
	
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		return false;
	}

	return true;
}

// Called before quitting
bool Logo::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(img);
	return true;
}