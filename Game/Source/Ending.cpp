#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Ending.h"
#include "EntityManager.h"
#include "Map.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Scene.h"
#include "Log.h"

Ending::Ending(bool startEnabled) : Module(startEnabled)
{
	name.Create("ending");
}

// Destructor
Ending::~Ending()
{}

// Called before render is available
bool Ending::Awake(pugi::xml_node& config)
{
	LOG("Loading Ending");
	bool ret = true;
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	endpath = (char*)config.child("pos").attribute("texturepath").as_string();
	x = config.child("pos").attribute("x").as_int();
	y = config.child("pos").attribute("y").as_int();
	return ret;
}

// Called before the first frame
bool Ending::Start()
{
	img = app->tex->Load(endpath);


	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->map->Disable();
	app->physics->Disable();

	return true;
}

// Called each loop iteration
bool Ending::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Ending::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->fade->FadeBlack(this, (Module*)app->titleScreen, 90);
	}

	return true;
}

// Called each loop iteration
bool Ending::PostUpdate()
{
	app->render->DrawTexture(img, x, y);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		return false;
	}

	return true;
}

// Called before quitting
bool Ending::CleanUp()
{
	LOG("Freeing scene");
	app->tex->UnLoad(img);
	return true;
}