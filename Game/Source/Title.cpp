#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Title.h"
#include "EntityManager.h"
#include "Map.h"
#include "Camera.h"
#include "FadeToBlack.h"
#include "Defs.h"
#include "Log.h"
#include "Player.h"

#include "Scene.h"

#include "Debug.h"


Title::Title(bool startEnabled) : Module(startEnabled)
{
	name.Create("title");
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
	pugi::xml_node node = app->GetNode();
	pugi::xml_node config = node.child(name.GetString());

	titlepath = (char*)config.child("pos").attribute("texturepath").as_string();
	animpath = (char*)config.child("pos").attribute("textureAnim").as_string();
	x = config.child("pos").attribute("x").as_int();
	y = config.child("pos").attribute("y").as_int();
	xA = config.child("pos").attribute("xA").as_int();
	yA = config.child("pos").attribute("yA").as_int();
	img = app->tex->Load(titlepath);
	animImg = app->tex->Load(animpath);

	app->camera->SetPosition(0, 0);

	anim.PushBack({ 0, 0, 15, 16 });
	anim.PushBack({ 16, 0, 15, 16 });
	anim.PushBack({ 32, 0, 15, 16 });
	anim.speed = 0.03f;

	run.PushBack({ 0, 16, 15, 16 });
	run.PushBack({ 16, 16, 15, 16 });
	run.PushBack({ 32, 16, 15, 16 });
	run.PushBack({ 48, 16, 15, 16 });
	run.PushBack({ 64, 16, 15, 16 });
	run.PushBack({ 80, 16, 15, 16 });
	run.PushBack({ 96, 16, 15, 16 });
	run.PushBack({ 112, 16, 15, 16 });
	run.speed = 0.15f;

	currentAnim = &anim;

	
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
		currentAnim = &run;
		
	}
	if (currentAnim == &run) {
		xA += 1;
	}
	if (xA >= 700) {
			app->fade->FadeBlack(this, (Module*)app->scene, 90);
	}
	return true;
}

// Called each loop iteration
bool Title::PostUpdate()
{
	app->render->DrawTexture(img, x, y, SDL_FLIP_NONE);
	SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(animImg, xA, yA, SDL_FLIP_NONE, &rect2);
	currentAnim->Update();

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