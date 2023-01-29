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
#include "Title.h"

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

	//FPS CAP
	app->debug->desiredFPS = config.child("frcap").attribute("fps").as_int();

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

	//TRIGGERS
	door_A = false;
	trapDoor_A = false;

	//COIN
	coin = (Coin*)app->entityManager->CreateEntity(EntityType::COIN);
	coin->parameters = config.child("coin");
	
	//HEART
	health = (Health*)app->entityManager->CreateEntity(EntityType::HEALTH);
	health->parameters = config.child("health");

	//Enables
	app->map->Enable();
	app->physics->Enable();
	app->pathFinding->Enable();
	app->entityManager->Enable();
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

	coinCount = config.child("ui").attribute("coinCount").as_int();
	healthCount = config.child("ui").attribute("healthCount").as_int();
	coinPath = (char*)config.child("ui").attribute("coinPath").as_string();
	healthPath = (char*)config.child("ui").attribute("healthPath").as_string();
	
	coinText = app->tex->Load(coinPath);
	healthText = app->tex->Load(healthPath);

	coinUI.PushBack({ 0, 0, 16, 16 });
	coinUI.PushBack({ 16, 0, 16, 16 });
	coinUI.PushBack({ 32, 0, 16, 16 });
	coinUI.PushBack({ 48, 0, 16, 16 });
	coinUI.PushBack({ 64, 0, 16, 16 });
	coinUI.PushBack({ 80, 0, 16, 16 });
	coinUI.PushBack({ 96, 0, 16, 16 });
	coinUI.PushBack({ 112, 0, 16, 16 });
	coinUI.speed = 0.03f;

	healthUI.PushBack({ 0, 0, 16, 16 });

	coinUIanim = &coinUI;
	healthUIanim = &healthUI;

	if (continue_pressed) {
		app->LoadGameRequest();
	}
	return true;
}


bool Scene::PreUpdate()
{
	OPTICK_EVENT();
	return true;
}


bool Scene::Update(float dt)
{
	OPTICK_EVENT();
	// Draw map
	app->map->Draw();
	
	//UI
	// Health
	app->render->DrawRectangle({ 0,0, 1472, 75 }, 255, 255, 255, 64, true, false);
	std::string string;
	
	SDL_Rect rect2 = healthUIanim->GetCurrentFrame();
	for (int i = 0; i < healthCount; i++) {
		app->render->Blit(healthText, 10+i*18, 10, &rect2, false);
	}
	
	//Score

	//Coins picked
	string = std::to_string(coinPicked);
	app->render->DrawText(string.c_str(), 215, 10, 50, 50, { 0, 0, 0 });
	SDL_Rect rect3 = coinUIanim->GetCurrentFrame();
	app->render->Blit(coinText, 90, 10, &rect3, false);
	coinUIanim->Update(dt);

	//Timer
	string = std::to_string((int)app->secondsSinceStartup);
	app->render->DrawText(string.c_str(), 700, 10, 50, 50, { 0, 0, 0 });

	//Check player death
	if (!player->alive)
	{
		app->fade->FadeBlack(this, (Module*)app->endScreen, 90);
	}

	if (coin->CheckPickingCoin()) {
		coinPicked++;
	}

	if (health->CheckPickingHealth()) {
		healthCount++;
	}
	return true;
}


bool Scene::PostUpdate()
{
	OPTICK_EVENT();
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
	continue_pressed = false;
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
	coin->SetPosition(data.child("coinItem").attribute("x").as_int(), data.child("coinItem").attribute("y").as_int());
	coin->alive = data.child("coinItem").attribute("alive").as_bool();
	health->SetPosition(data.child("healthItem").attribute("x").as_int(), data.child("healthItem").attribute("y").as_int());
	health->alive = data.child("healthItem").attribute("alive").as_bool();

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

	pugi::xml_node coinItem = data.append_child("coin");
	coinItem.append_attribute("x") = coin->position.x;
	coinItem.append_attribute("y") = coin->position.y;
	coinItem.append_attribute("alive") = coin->alive;

	pugi::xml_node healthItem = data.append_child("health");
	healthItem.append_attribute("x") = health->position.x;
	healthItem.append_attribute("y") = health->position.y;
	healthItem.append_attribute("alive") = health->alive;

	return true;
}