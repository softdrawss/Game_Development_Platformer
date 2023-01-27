#include "GuiSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Scene.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	//audioFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");
}

GuiSlider::~GuiSlider()
{

}

bool GuiSlider::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {
			
			state = GuiControlState::FOCUSED;
			if (previousState != state) {
				LOG("Change state from %d to %d",previousState,state);
				//app->audio->PlayFx(audioFxId);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN) {
				state = GuiControlState::PRESSED;
				
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




bool GuiSlider::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State

	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(bounds, 200, 200, 200, 200, true, false);
		break;
	case GuiControlState::NORMAL:
		render->DrawRectangle(bounds, 0, 0, 255, 200, true, false);
		break;
	case GuiControlState::FOCUSED:
		render->DrawRectangle(bounds, 153, 153, 255, 200, true, false);
		break;
	case GuiControlState::PRESSED:
		render->DrawRectangle(bounds, 0, 255, 0, 200, true, false);
		break;
	}

	app->render->DrawText(text.GetString(), bounds.x+10, bounds.y+5, bounds.w-20, bounds.h-10, {255,255,255});

	return false;
}