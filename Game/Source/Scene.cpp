#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Fonts.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "PathFinding.h"
#include "EntityManager.h"
#include "Map.h"
#include "Camera.h"
#include "Debug.h"
#include "FadeToBlack.h"
#include "Particles.h"
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

	
	//ENEMIES
	enemyWalk = (EnemyWalk*)app->entityManager->CreateEntity(EntityType::WALK);
	enemyWalk->parameters = config.child("walk");

	enemyFly = (EnemyFly*)app->entityManager->CreateEntity(EntityType::FLY);
	enemyFly->parameters = config.child("fly");

	//NPC
	npcChickBoy = (NPCChickBoy*)app->entityManager->CreateEntity(EntityType::NPC);
	npcChickBoy->parameters = config.child("chickboy");

	//PLAYER
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");
	
	//PLAYER INTERACT
	playerInteract = (PlayerInteract*)app->entityManager->CreateEntity(EntityType::INTERACT);
	playerInteract->parameters = config.child("interact");

	//Enables
	app->map->Enable();
	app->physics->Enable();
	app->pathFinding->Enable();
	app->entityManager->Enable();
	app->debug->Enable();
	app->particles->Enable();

	// Iterate all objects in the scene -- Check https://pugixml.org/docs/quickstart.html#access	
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	//MAP
	if (app->map->Load())
	{
		int w, h;
		uchar* buffer = NULL;

		if (app->map->CreateWalkabilityMap(w, h, &buffer))
			app->pathFinding->SetMap(w, h, buffer);

		RELEASE_ARRAY(buffer);
	}

	//MUSIC
	musicPath = (char*)config.child("music").attribute("audioPath").as_string();
	app->audio->PlayMusic(musicPath, 1.0F);
	
	//FONTS
	fontPath = (char*)config.child("fonts").attribute("fontPath").as_string();
	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz 0123456789.,;:$#'! /?%&()@ -+=      " };
	app->fonts->font_white = app->fonts->Load(fontPath, lookupTable, 7);

	//CAMERA
	app->camera->cameraMode = PLAYER;
	speed = config.child("camera").attribute("speed").as_int();
	camX = config.child("camera").attribute("x").as_int();
	camY = config.child("camera").attribute("y").as_int();
	/*app->render->camera.x = camX;
	app->render->camera.y = camY * app->win->GetScale();
	camSpeed = speed * app->win->GetScale();*/

	//WINDOW
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());
	
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
	app->map->Disable();
	app->entityManager->Disable();
	app->physics->Disable();

	return true;
}

// L03: DONE 6: Implement a method to load the state load players's x and y
bool Scene::LoadState(pugi::xml_node& data)
{
	player->SetPosition(data.child("player").attribute("x").as_int(), data.child("player").attribute("y").as_int());
	enemyWalk->SetPosition(data.child("walk").attribute("x").as_int(), data.child("walk").attribute("y").as_int());
	enemyWalk->alive = data.child("walk").attribute("alive").as_bool(); 
	enemyFly->SetPosition(data.child("fly").attribute("x").as_int(), data.child("fly").attribute("y").as_int());
	enemyFly->alive = data.child("fly").attribute("alive").as_bool();


	return true;
}

// L03: DONE 8: Create a method to save the state of the player
// using append_child and append_attribute
bool Scene::SaveState(pugi::xml_node& data)
{
	pugi::xml_node play = data.append_child("player");

	play.append_attribute("x") = player->position.x + 16;
	play.append_attribute("y") = player->position.y - 10;

	pugi::xml_node walk = data.append_child("walk");
	walk.append_attribute("x") = enemyWalk->position.x + 16;
	walk.append_attribute("y") = enemyWalk->position.y - 10;
	walk.append_attribute("alive") = enemyWalk->alive;

	pugi::xml_node fly = data.append_child("fly");
	fly.append_attribute("x") = enemyFly->position.x + 16;
	fly.append_attribute("y") = enemyFly->position.y - 10;
	fly.append_attribute("alive") = enemyFly->alive;

	return true;
}