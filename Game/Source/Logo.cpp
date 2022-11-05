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

#include "Defs.h"
#include "Log.h"

Logo::Logo(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
}

// Destructor
Logo::~Logo()
{}

// Called before render is available
bool Logo::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Logo::Start()
{
	img = app->tex->Load("Assets/Textures/logo.png");

	app->render->camera.x = 0;
	app->render->camera.y = 0;

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
	app->fade->FadeBlack(this, (Module*)app->scene, 30);


	return true;
}

// Called each loop iteration
bool Logo::PostUpdate()
{
	app->render->DrawTexture(img, 0, 0);

	if (app->input->GetKey(SDL_SCANCODE_SPACE == KEY_REPEAT))
	{
	}
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE == KEY_REPEAT))
	{
		return false;
	}

	return true;
}

// Called before quitting
bool Logo::CleanUp()
{
	LOG("Freeing scene");

	return true;
}