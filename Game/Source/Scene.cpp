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
#include "GuiManager.h"

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
	app->debug->controlFPS = config.child("vsync").attribute("value").as_int();

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
	score = config.child("ui").attribute("score").as_int();

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

	B_resume = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 2, "RESUME", { 586,350 + 25,300,90 }, this);
	B_resume->button = GuiButtontype::RESUME;
	B_resume->active = false;
	B_resume->state = GuiControlState::DISABLED;
	B_settings = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 2, "SETTINGS", { 586,450 + 35,300,90 }, this);
	B_settings->button = GuiButtontype::SETTINGS;
	B_settings->active = false;
	B_settings->state = GuiControlState::DISABLED;
	B_back_to_title = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 2, "BACK TO TITLE", { 586,550 + 45,300,90 }, this);
	B_back_to_title->button = GuiButtontype::BACK_TO_TITLE;
	B_back_to_title->active = false;
	B_back_to_title->state = GuiControlState::DISABLED;
	B_exit = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 2, "EXIT", { 586,650 + 55,300,90 }, this);
	B_exit->button = GuiButtontype::EXIT;
	B_exit->active = false;
	B_exit->state = GuiControlState::DISABLED;

	B_back = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 2, "BACK", { 1254,8,100,50 }, this);
	B_back->button = GuiButtontype::BACK;
	B_back->active = false;
	B_back->state = GuiControlState::DISABLED;

	//Sliders
	S_music = (GuiSlider*)app->guimanager->CreateGuiControl(GuiControlType::SLIDER, 2, "MUSIC", { 260,250,40,40 }, this, { 230, 257, 120, 10 });
	S_music->slider = GuiSliderType::MUSIC;
	S_music->active = false;
	S_music->state = GuiControlState::DISABLED;
	S_fx = (GuiSlider*)app->guimanager->CreateGuiControl(GuiControlType::SLIDER, 2, "FX", { 260,270 + 50,40,40 }, this, { 230, 327, 120, 10 });
	S_fx->slider = GuiSliderType::FX;
	S_fx->active = false;
	S_fx->state = GuiControlState::DISABLED;

	//CheckBox
	C_screen = (GuiCheckBox*)app->guimanager->CreateGuiControl(GuiControlType::CHECKBOX, 2, "FULL SCREEN", { 260,390,50,50 }, this);
	C_screen->button = GuiCheckBoxType::FULLSCREEN;
	C_screen->active = false;
	C_screen->state = GuiControlState::DISABLED;
	C_vysinc = (GuiCheckBox*)app->guimanager->CreateGuiControl(GuiControlType::CHECKBOX, 2, "VSYNC", { 260,460,50,50 }, this);
	C_vysinc->button = GuiCheckBoxType::VSYNC;
	C_vysinc->active = false;
	C_vysinc->state = GuiControlState::DISABLED;


	pause = false;
	settings = false;
	exit = false;
	return true;
}

void Scene::PauseMenu() {
	B_resume->state = GuiControlState::NORMAL;
	B_resume->active = true;
	B_settings->state = GuiControlState::NORMAL;
	B_settings->active = true;
	B_back_to_title->state = GuiControlState::NORMAL;
	B_back_to_title->active = true;
	B_exit->state = GuiControlState::NORMAL;
	B_exit->active = true;
	//DrawSettings();
	//settings = true;
}

void Scene::DrawSettings() {
	B_resume->state = GuiControlState::DISABLED;
	B_settings->state = GuiControlState::DISABLED;
	B_back_to_title->state = GuiControlState::DISABLED;
	B_exit->state = GuiControlState::DISABLED;

	B_back->active = true;
	B_back->state = GuiControlState::NORMAL;
	S_music->active = true;
	S_music->state = GuiControlState::NORMAL;
	S_fx->active = true;
	S_fx->state = GuiControlState::NORMAL;
	C_screen->active = true;
	C_screen->state = GuiControlState::NORMAL;
	C_vysinc->active = true;
	C_vysinc->state = GuiControlState::NORMAL;

	app->render->DrawRectangle({ 400, 58, 1236 - (400 - 118), 738 }, 255, 79, 120, 255, true, false);
	app->render->DrawText("SETTINGS", 423 + 118, 58 + 34, 389, 118, { 244,244,228 });

	app->render->DrawText("MUSIC VOLUME", 343 + 70, 200 + 50, 250, 50, { 244,244,228 });
	app->render->DrawText("FX VOLUME", 343 + 70, 270 + 50, 175, 50, { 244,244,228 });
	app->render->DrawText("FULLSCREEN", 343 + 70, 340 + 50, 245, 50, { 244,244,228 });
	app->render->DrawText("VSYNC", 343 + 70, 410 + 50, 150, 50, { 244,244,228 });

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
	string = "Score: "+ std::to_string(score);
	app->render->DrawText(string.c_str(), 1200, 10, 200, 50, { 0, 0, 0 });

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
		score += 50;
	}

	if (health->CheckPickingHealth()) {
		healthCount++;
	}
	if ((int)app->secondsSinceStartup <= 5000) {
		player->alive = false;
	}

	return true;
}


bool Scene::PostUpdate()
{
	OPTICK_EVENT();
	app->guimanager->Draw();

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) {
		app->map->Draw();

		pause = true;
		PauseMenu();
	}

	if (pause && settings) {
		DrawSettings();
		B_resume->state = GuiControlState::DISABLED;
		B_settings->state = GuiControlState::DISABLED;
		B_back_to_title->state = GuiControlState::DISABLED;
		B_exit->state = GuiControlState::DISABLED;
	}
	if (pause && !settings) {
		B_back->active = false;
		B_back->state = GuiControlState::DISABLED;
		S_music->active = false;
		S_music->state = GuiControlState::DISABLED;
		S_fx->active = false;
		S_fx->state = GuiControlState::DISABLED;
		C_screen->active = false;
		C_screen->state = GuiControlState::DISABLED;
		C_vysinc->active = false;
		C_vysinc->state = GuiControlState::DISABLED;
		B_resume->state = GuiControlState::NORMAL;
		B_settings->state = GuiControlState::NORMAL;
		B_back_to_title->state = GuiControlState::NORMAL;
		B_exit->state = GuiControlState::NORMAL;
	}
	if (pause && exit)
		return false;

	if (!pause) {
		settings = false;
		exit = false;
		B_resume->state = GuiControlState::DISABLED;
		B_resume->active = false;
		B_settings->state = GuiControlState::DISABLED;
		B_settings->active = false;
		B_back_to_title->state = GuiControlState::DISABLED;
		B_back_to_title->active = false;
		B_exit->state = GuiControlState::DISABLED;
		B_exit->active = false;
	}
	/*else {
		
	}*/
	
	

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
	B_resume->state = GuiControlState::DISABLED;
	B_resume->active = false;
	B_settings->state = GuiControlState::DISABLED;
	B_settings->active = false;
	B_back_to_title->state = GuiControlState::DISABLED;
	B_back_to_title->active = false;
	B_exit->state = GuiControlState::DISABLED;
	B_exit->active = false;
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
	coinCount = data.child("uiData").attribute("coinCount").as_int();
	healthCount = data.child("uiData").attribute("healthCount").as_int();
	score = data.child("uiData").attribute("score").as_int();

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

	pugi::xml_node uiData = data.append_child("ui");
	uiData.append_attribute("coinCount") = coinCount;
	uiData.append_attribute("healthCount") = healthCount;
	uiData.append_attribute("score") = score;

	return true;
}