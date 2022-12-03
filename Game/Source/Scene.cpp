#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Fonts.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Camera.h"
#include "Debug.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"

Scene::Scene(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
	active = false;
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	pugi::xml_node node = app->GetNode();
	pugi::xml_node config = node.child(name.GetString());

	//PLAYER
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

	//Enables
	app->map->Enable();
	app->physics->Enable();
	app->entityManager->Enable();
	app->camera->Enable();
	app->debug->Enable();

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	
	

	//MUSIC
	musicPath = (char*)config.child("music").attribute("audioPath").as_string();
	//app->audio->PlayMusic(musicPath, 1.0F);
	
	//FONTS
	fontPath = (char*)config.child("fonts").attribute("fontPath").as_string();
	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz 012+3456789.,;:$#'! /?%&()@ -+=      " };
	app->fonts->font_white = app->fonts->Load(fontPath, lookupTable, 7);

	//CAMERA
	speed = config.child("camera").attribute("speed").as_int();
	camX = config.child("camera").attribute("x").as_int();
	camY = config.child("camera").attribute("y").as_int();
	/*app->render->camera.x = camX;
	app->render->camera.y = camY * app->win->GetScale();
	camSpeed = speed * app->win->GetScale();*/


	//MAP
	app->map->Load();

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());
	
	//WINDOW
	app->win->SetTitle(title.GetString());

	return true;
}


bool Scene::PreUpdate()
{
	return true;
}


bool Scene::Update(float dt)
{
	// Draw map
	app->map->Draw();

	//Check player death
	if (!player->alive)
	{
		app->fade->FadeBlack(this, (Module*)app->endScreen, 90);
	}

	return true;
}


bool Scene::PostUpdate()
{
	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		return false;

	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

// L03: DONE 6: Implement a method to load the state load players's x and y
bool Scene::LoadState(pugi::xml_node& data)
{
	player->SetPosition(data.child("player").attribute("x").as_int(), data.child("player").attribute("y").as_int());

	return true;
}

// L03: DONE 8: Create a method to save the state of the player
// using append_child and append_attribute
bool Scene::SaveState(pugi::xml_node& data)
{
	pugi::xml_node play = data.append_child("player");

	play.append_attribute("x") = player->position.x + 16;
	play.append_attribute("y") = player->position.y - 10;

	return true;
}