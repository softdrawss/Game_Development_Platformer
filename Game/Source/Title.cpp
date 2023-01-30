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
#include "GuiManager.h"
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
	app->debug->Enable();

	pugi::xml_node node = app->GetNode();
	pugi::xml_node config = node.child(name.GetString());

	titlepath = (char*)config.child("pos").attribute("texturepath").as_string();
	animpath = (char*)config.child("pos").attribute("textureAnim").as_string();
	credpath = (char*)config.child("pos").attribute("credits").as_string();
	x = config.child("pos").attribute("x").as_int();
	y = config.child("pos").attribute("y").as_int();
	xA = config.child("pos").attribute("xA").as_int();
	yA = config.child("pos").attribute("yA").as_int();

	//FPS CAP
	app->debug->desiredFPS = config.child("frcap").attribute("fps").as_int();
	app->debug->controlFPS = config.child("vsync").attribute("value").as_int();
	app->SetMaxFrameDuration((float)1000 / app->debug->desiredFPS);

	img = app->tex->Load(titlepath);
	animImg = app->tex->Load(animpath);
	credImg = app->tex->Load(credpath);

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

	//Buttons
	B_play = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 2, "PLAY", { 586,250+15,300,90 }, this);
	B_play->button = GuiButtontype::PLAY;
	B_play->active = true;
	B_continue = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 2, "CONTINUE", { 586,350+25,300,90 }, this);
	B_continue->button = GuiButtontype::CONTINUE;
	B_continue->state = GuiControlState::DISABLED;
	B_continue->active = true;
	B_settings = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 2, "SETTINGS", { 586,450+35,300,90 }, this);
	B_settings->button = GuiButtontype::SETTINGS;
	B_settings->active = true;
	B_credits = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 2, "CREDITS", { 586,550+45,300,90 }, this);
	B_credits->button = GuiButtontype::CREDITS;
	B_credits->active = true;
	B_exit = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 2, "EXIT", { 586,650+55,300,90 }, this);
	B_exit->button = GuiButtontype::EXIT;
	B_exit->active = true;

	B_back = (GuiButton*)app->guimanager->CreateGuiControl(GuiControlType::BUTTON, 2, "BACK", { 1254,8,100,50 }, this);
	B_back->button = GuiButtontype::BACK;
	B_back->active = false;
	B_back->state = GuiControlState::DISABLED;

	//Sliders
	S_music = (GuiSlider*)app->guimanager->CreateGuiControl(GuiControlType::SLIDER, 2, "MUSIC", { 260,250,40,40 }, this, { 230, 257, 120, 10 });
	S_music->slider = GuiSliderType::MUSIC;
	S_music->active = false;
	S_music->state = GuiControlState::DISABLED;
	S_fx = (GuiSlider*)app->guimanager->CreateGuiControl(GuiControlType::SLIDER, 2, "FX", { 260,270+50,40,40 }, this, { 230, 327, 120, 10 });
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


	credits = false;
	settings = false;
	if (CheckContinue()) {
		B_continue->state = GuiControlState::NORMAL;
	}

	return true;
}

//Check if we can continue the game or not
bool Title::CheckContinue() {
	
	return app->CheckFileExists();
}

// Called each loop iteration
bool Title::PreUpdate()
{
	OPTICK_EVENT();
	return true;
}

// Called each loop iteration
bool Title::Update(float dt)
{
	OPTICK_EVENT();

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		currentAnim = &run;		
	}
	if (currentAnim == &run)
	{
		xA += 1;
	}
	if (xA >= 700)
	{
		app->fade->FadeBlack(this, (Module*)app->scene, 90);
	}

	if (exit)
		return false;

	return true;
}

// Called each loop iteration
bool Title::PostUpdate()
{
	OPTICK_EVENT();
	app->render->DrawTexture(img, x, y, SDL_FLIP_NONE);
	SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(animImg, xA, yA, SDL_FLIP_NONE, &rect2);
	currentAnim->Update(app->GetDeltaTime());
	app->guimanager->Draw();

	if (credits)
		DrawCredits();
	else if (settings)
		DrawSettings();
	else {
		//B_play->state = GuiControlState::NORMAL;
		//B_continue->state = GuiControlState::NORMAL;
		//B_settings->state = GuiControlState::NORMAL;
		//B_credits->state = GuiControlState::NORMAL;
		//B_exit->state = GuiControlState::NORMAL;
		
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
	}
	return true;
}

// Called before quitting
bool Title::CleanUp()
{
	LOG("Freeing title");
	app->tex->UnLoad(img);

	B_play->state = GuiControlState::DISABLED;
	B_play->active = false;
	B_continue->state = GuiControlState::DISABLED;
	B_continue->active = false;
	B_settings->state = GuiControlState::DISABLED;
	B_settings->active = false;
	B_credits->state = GuiControlState::DISABLED;
	B_credits->active = false;
	B_exit->state = GuiControlState::DISABLED;
	B_exit->active = false;

	return true;
}

void Title::DrawCredits() {
	//
	B_play->state = GuiControlState::DISABLED;
	B_continue->state = GuiControlState::DISABLED;
	B_settings->state = GuiControlState::DISABLED;
	B_credits->state = GuiControlState::DISABLED;
	B_exit->state = GuiControlState::DISABLED;

	B_back->active = true;
	B_back->state = GuiControlState::NORMAL;
	app->render->DrawRectangle({ 118, 58, 1236, 738 }, 255, 79, 120, 255, true, false);
	//app->render->DrawText("CREDITS", 423 + 118, 58 + 34, 389, 118, { 244,244,228 });
	app->render->DrawTexture(credImg, 59, 29, SDL_FLIP_NONE);
	SDL_Rect rect = { 0, 0, 1236, 738 };
	//NO se ve nada
}

void Title::DrawSettings() {
	B_play->state = GuiControlState::DISABLED;
	B_continue->state = GuiControlState::DISABLED;
	B_settings->state = GuiControlState::DISABLED;
	B_credits->state = GuiControlState::DISABLED;
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

	app->render->DrawRectangle({ 400, 58, 1236-(400-118), 738 }, 255, 79, 120, 255, true, false);
	app->render->DrawText("SETTINGS", 423 + 118, 58 + 34, 389, 118, { 244,244,228 });

	app->render->DrawText("MUSIC VOLUME", 343+70, 200+50, 250, 50, { 244,244,228 });
	app->render->DrawText("FX VOLUME", 343 + 70, 270 + 50, 175, 50, { 244,244,228 });
	app->render->DrawText("FULLSCREEN", 343 + 70, 340 + 50, 245, 50, { 244,244,228 });
	app->render->DrawText("VSYNC", 343 + 70, 410 + 50, 150, 50, { 244,244,228 });
	
}

// Define multiple Gui Event methods
bool Title::OnGuiMouseClickEvent(GuiControl* control) {
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		LOG("Button 1 click");
		break;
	case 2:
		LOG("Button 2 click");
		break;
	}

	return true;

}