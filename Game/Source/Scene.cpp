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

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access
	for (pugi::xml_node itemNode = config.child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
	}

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = config.child("player");

	app->audio->PlayMusic("Assets/Audio/Music/platform.ogg", 1.0F);

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	app->map->Enable();
	app->entityManager->Enable();
	app->debug->Enable();

	player->alive = true;
	camSpeed = 6 * app->win->GetScale();


	player->pbody->body->SetTransform(PIXEL_TO_METERS(player->initPosition), 0);

	char lookupTable[] = { "abcdefghijklmnopqrstuvwxyz 0123456789.,;:$#'! /?%&()@ -+=      " };
	app->fonts->font_white = app->fonts->Load("Assets/Textures/sprite_font_white.png", lookupTable, 7);
	app->render->camera.x = 0;
	app->render->camera.y = -448* app->win->GetScale();

	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	
	// L03: DONE: Load map
	app->map->Load();

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());
	
	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	int scale = app->win->GetScale();
	
	if (!app->debug->freeCam)
	{
		//Camera behaviour
		//Left
		if (player->position.x * scale + app->render->camera.x < app->render->camera.w * 0.4)
			app->render->camera.x = -player->position.x * scale + (app->render->camera.w * 0.4);
		//right
		else if (player->position.x * scale + app->render->camera.x > app->render->camera.w * 0.6)
			app->render->camera.x = -player->position.x * scale + (app->render->camera.w * 0.6);
		//up
		if (player->position.y * scale + app->render->camera.y < app->render->camera.h * 0.4)
			app->render->camera.y = -player->position.y * scale + (app->render->camera.h * 0.4);
		//down
		else if (player->position.y * scale + app->render->camera.y > app->render->camera.h * 0.6)
			app->render->camera.y = -player->position.y * scale + (app->render->camera.h * 0.6);


		//Camera limits
		if (app->render->camera.x > 0)
			app->render->camera.x = 0;
		else if (app->render->camera.x < -3310)
			app->render->camera.x = -3310;
		if (app->render->camera.y > -352)
			app->render->camera.y = -352;
		else if (app->render->camera.y < -448)
			app->render->camera.y = -448;
	}
	else
	{
		//Free Camera
		if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			app->render->camera.y += camSpeed;

		if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			app->render->camera.y -= camSpeed;

		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			app->render->camera.x += camSpeed;

		if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			app->render->camera.x -= camSpeed;
	}
	

	// Draw map
	app->map->Draw();


	if (!player->alive)
	{
		app->fade->FadeBlack(this, (Module*)app->endScreen, 90);
	}

	return true;
}

// Called each loop iteration
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