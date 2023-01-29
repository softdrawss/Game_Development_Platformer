#include "GuiSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Scene.h"
#include "Debug.h"
#include "Window.h"
#include "SDL/include/SDL.h"
#include "SDL_mixer/include/SDL_mixer.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text, SDL_Rect sliderBounds) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	this->sliderbounds = sliderBounds;

	canClick = true;
	drawBasic = false;

	bounds.x = sliderbounds.x + sliderbounds.w;

	focused = app->audio->LoadFx("Assets/Audio/Fx/focused.wav");
	pressed = app->audio->LoadFx("Assets/Audio/Fx/click.wav");
	this->state = GuiControlState::NORMAL;
}

GuiSlider::~GuiSlider()
{
}

bool GuiSlider::Update(float dt)
{
	int scale = app->win->GetScale();
	if (canClick == false) {
		state = GuiControlState::NORMAL;
		return false;
	}
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;
		state = GuiControlState::NORMAL;

		// I'm inside the limitis of the button
		if (mouseX * scale>= bounds.x && mouseX * scale <= bounds.x + bounds.w &&
			mouseY * scale >= bounds.y && mouseY * scale <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;
			app->audio->PlayFx(focused);


			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
				//app->audio->PlayFx(pressed);

			}


			if (previousState == GuiControlState::PRESSED && state == GuiControlState::NORMAL) {
				//in case you want to change the volume in intervals of 25
				/*if (mouseX > sliderbounds.x + 12.5f && mouseX < sliderbounds.x + 37.5f) {
					bounds.x = sliderbounds.x + 25 - bounds.w / 2;
				}else if (mouseX > sliderbounds.x + 37.5f && mouseX < sliderbounds.x + 62.5f) {
					bounds.x = sliderbounds.x + 50 - bounds.w/2;
				}
				else if (mouseX > sliderbounds.x + 62.5f && mouseX < sliderbounds.x + 87.5f) {
					bounds.x = sliderbounds.x + 75 - bounds.w / 2;
				}
				else if(mouseX < sliderbounds.x + 12.5f && mouseX > sliderbounds.x -1){
					bounds.x = sliderbounds.x;
				}
				else if (mouseX > sliderbounds.x + 87.5f && mouseX < sliderbounds.x +sliderbounds.w +1) {
					bounds.x = sliderbounds.x + 100 - bounds.w/2;
				}
				else if (mouseX > sliderbounds.x + 100.0f) {
					bounds.x = sliderbounds.x + sliderbounds.w;
				}*/
			}
			else if (previousState == GuiControlState::NORMAL && state == GuiControlState::FOCUSED) {
				LOG("Change state from %d to %d", previousState, state);
				app->audio->PlayFx(focusaudioFxId);
			}

			if (previousState == GuiControlState::FOCUSED && state == GuiControlState::PRESSED) {
				app->audio->PlayFx(clickaudioFxId);
			}

			if (state == GuiControlState::PRESSED) {
				bounds.x = (mouseX*scale - (bounds.w / 2));
				if (bounds.x > sliderbounds.x + sliderbounds.w) {
					bounds.x = sliderbounds.x + sliderbounds.w;
				}
				else if (bounds.x < sliderbounds.x) {
					bounds.x = sliderbounds.x;
				}
			}

			if (slider == GuiSliderType::MUSIC) {
				Mix_VolumeMusic(bounds.x - sliderbounds.x);
			}
			if (slider == GuiSliderType::FX) {
				Mix_Volume(-1, bounds.x - sliderbounds.x);
			}
		}
	}
}

bool GuiSlider::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State
	app->input->GetMousePosition(mouseX, mouseY);
	if (active) {
		if (app->debug->colourblind) {
			switch (state)
			{
			case GuiControlState::DISABLED:

				app->render->DrawRectangle(sliderbounds, 255, 255, 255, 255, true, false);
				app->render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 0, 0, 0, 255, true, false);

				break;
			case GuiControlState::NORMAL:
				app->render->DrawRectangle(sliderbounds, 255, 255, 255, 255, true, false);
				app->render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 0, 0, 255, 255, true, false);

				break;
			case GuiControlState::FOCUSED:
				//section of the rectangle
				app->render->DrawRectangle(sliderbounds, 255, 255, 255, 255, true, false);
				app->render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 0, 255, 0, 255, true, false);

				break;
			case GuiControlState::PRESSED:
				app->render->DrawRectangle(sliderbounds, 255, 255, 255, 255, true, false);
				app->render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 255, 0, 0, 255, true, false);

				break;
			}
		}
		else {
			switch (state)
			{
			case GuiControlState::DISABLED:

				app->render->DrawRectangle(sliderbounds, 255, 255, 255, 255, true, false);
				app->render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 0, 0, 0, 255, true, false);

				break;
			case GuiControlState::NORMAL:
				app->render->DrawRectangle(sliderbounds, 255, 255, 255, 255, true, false);
				app->render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 255, 79, 120, 255, true, false);

				break;
			case GuiControlState::FOCUSED:
				//section of the rectangle
				app->render->DrawRectangle(sliderbounds, 255, 255, 255, 255, true, false);
				app->render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 83, 94, 105, 255, true, false);

				break;
			case GuiControlState::PRESSED:
				app->render->DrawRectangle(sliderbounds, 255, 255, 255, 255, true, false);
				app->render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 255, 0, 0, 255, true, false);

				break;
			}
		}
	}
	
	return false;
}

