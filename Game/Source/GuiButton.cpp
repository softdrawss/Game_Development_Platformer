#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Scene.h"
#include "FadeToBlack.h"
#include "Title.h"
#include "Scene.h"
#include "Physics.h"
#include "Window.h"
#include "Debug.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	focused = app->audio->LoadFx("Assets/Audio/Fx/focused.wav");
	pressed = app->audio->LoadFx("Assets/Audio/Fx/click.wav");
	this->state = GuiControlState::NORMAL;
	active = false;
}

GuiButton::~GuiButton()
{

}

bool GuiButton::Update(float dt)
{
	int scale = app->win->GetScale();
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition((mouseX), mouseY);

		//mouseX = PIXEL_TO_METERS(mouseX);
		//mouseY = PIXEL_TO_METERS(mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX * scale >= bounds.x && mouseX * scale <= bounds.x + bounds.w &&
			mouseY * scale >= bounds.y && mouseY * scale <= bounds.y + bounds.h) {
			
			state = GuiControlState::FOCUSED;
			if (previousState != state) {
				LOG("Change state from %d to %d",previousState,state);
				app->audio->PlayFx(focused);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN) {
				state = GuiControlState::PRESSED;
				app->audio->PlayFx(pressed);
				switch (this->button)
				{
				case GuiButtontype::PLAY:
					app->fade->FadeBlack((Module*)app->titleScreen, (Module*)app->scene, 90);
					app->audio->PlayFx(pressed);

					break;
				case GuiButtontype::CONTINUE:
					app->fade->FadeBlack((Module*)app->titleScreen, (Module*)app->scene, 90);
					app->audio->PlayFx(pressed);
					app->scene->continue_pressed = true;
					break;
				case GuiButtontype::SETTINGS:
					app->titleScreen->settings = true;
					app->scene->settings = true;
					break;
				case GuiButtontype::CREDITS:
					app->titleScreen->credits = true;
					break;
				case GuiButtontype::RESUME:
					app->scene->pause = false;
					break;
				case GuiButtontype::BACK_TO_TITLE:
					app->fade->FadeBlack((Module*)app->scene, (Module*)app->titleScreen, 90);
					break;
				case GuiButtontype::EXIT:
					app->titleScreen->exit = true;
					app->scene->exit = true;
					break;
				case GuiButtontype::BACK:
					app->titleScreen->credits = false;
					app->titleScreen->settings = false;
					app->scene->settings = false;
					break;
				default:
					break;
				}
			}

			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}
	}

	return false;
}




bool GuiButton::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State
	if (active) {
		if (app->debug->colourblind) {
			switch (state)
			{
			case GuiControlState::DISABLED:
				render->DrawRectangle(bounds, 255, 234, 0, 200, true, false);
				break;
			case GuiControlState::NORMAL:
				render->DrawRectangle(bounds, 255, 0, 0, 200, true, false);
				break;
			case GuiControlState::FOCUSED:
				render->DrawRectangle(bounds, 0, 0, 225, 200, true, false);
				break;
			case GuiControlState::PRESSED:
				render->DrawRectangle(bounds, 0, 255, 0, 200, true, false);
				break;
			}

			app->render->DrawText(text.GetString(), bounds.x + 10, bounds.y + 5, bounds.w - 20, bounds.h - 10, { 244,244,228 });
		}
		else {
			switch (state)
			{
			case GuiControlState::DISABLED:
				render->DrawRectangle(bounds, 71, 176, 206, 200, true, false);
				break;
			case GuiControlState::NORMAL:
				render->DrawRectangle(bounds, 255, 79, 120, 200, true, false);
				break;
			case GuiControlState::FOCUSED:
				render->DrawRectangle(bounds, 83, 94, 105, 200, true, false);
				break;
			case GuiControlState::PRESSED:
				render->DrawRectangle(bounds, 31, 35, 47, 200, true, false);
				break;
			}

			app->render->DrawText(text.GetString(), bounds.x + 10, bounds.y + 5, bounds.w - 20, bounds.h - 10, { 244,244,228 });
		}
	}

	return false;
}