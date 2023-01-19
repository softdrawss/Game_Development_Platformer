#include "InpButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "SString.h"
#include "Scene.h"

InpButton::InpButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::INPUTBOX, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	//audioFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
	//state = GuiControlState::NORMAL;
	input = "0.00000";
	renderText = false;
}

InpButton::~InpButton()
{
}

bool InpButton::Update(float dt)
{
	renderText = false;
	SDL_Event e;

	
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

			if (previousState != state) {
				LOG("Change state from %d to %d", previousState, state);
				//app->audio->PlayFx(audioFxId);
			}

			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				state = GuiControlState::PRESSED;
				app->scene->isButtonPressed = true;
				SDL_StartTextInput();

			}			
		}
		if (!(mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) && (state == GuiControlState::NORMAL) && app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
			state = GuiControlState::DISABLED;

		}
		if (state == GuiControlState::PRESSED) {
			if ((app->input->GetKey(SDL_SCANCODE_BACKSPACE) == KEY_UP) && input.length() > 0) {
				input.pop_back();
				renderText = true;
			}
			if ((app->input->GetKey(SDL_SCANCODE_1) == KEY_UP)) {
				std::string n = "1";
				input += n;
				renderText = true;

			} if ((app->input->GetKey(SDL_SCANCODE_2) == KEY_UP)) {
				std::string n = "2";
				input += n;
				renderText = true;
			} if ((app->input->GetKey(SDL_SCANCODE_3) == KEY_UP)) {
				std::string n = "3";
				input += n;
				renderText = true;
			} if ((app->input->GetKey(SDL_SCANCODE_4) == KEY_UP)) {
				std::string n = "4";
				input += n;
				renderText = true;
			} if ((app->input->GetKey(SDL_SCANCODE_5) == KEY_UP)) {
				std::string n = "5";
				input += n;
				renderText = true;
			} if ((app->input->GetKey(SDL_SCANCODE_6) == KEY_UP)) {
				std::string n = "6";
				input += n;
				renderText = true;
			} if ((app->input->GetKey(SDL_SCANCODE_7) == KEY_UP)) {
				std::string n = "7";
				input += n;
				renderText = true;
			} if ((app->input->GetKey(SDL_SCANCODE_8) == KEY_UP)) {
				std::string n = "8";
				input += n;
				renderText = true;
			} if ((app->input->GetKey(SDL_SCANCODE_9) == KEY_UP)) {
				std::string n = "9";
				input += n;
				renderText = true;
			} if ((app->input->GetKey(SDL_SCANCODE_0) == KEY_UP)) {
				std::string n = "0";
				input += n;
				renderText = true;
			}if ((app->input->GetKey(SDL_SCANCODE_PERIOD) == KEY_UP)) {
				std::string n = ".";
				input += n;
				renderText = true;
			}if ((app->input->GetKey(SDL_SCANCODE_MINUS) == KEY_UP)) {
				std::string n = "-";
				input += n;
				renderText = true;
			}	if ((app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP)) {
				state = GuiControlState::NORMAL;
				app->scene->isButtonPressed = false;

			}
		}
	

	return false;
}




bool InpButton::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State

	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(bounds, 204, 204, 204, 200, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(bounds, 204, 204, 204, 200, true, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(bounds, 204, 204, 204, 200, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(bounds, 0, 204, 204, 200, true, false);
		break;
	}

	if (renderText = true) {
		app->render->DrawText(input.c_str(), bounds.x, bounds.y, bounds.w, bounds.h, { 255,255,255 });
	}
	

	return false;
}
