#include "GuiCheckBox.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Log.h"
#include "Scene.h"
#include "Window.h"
#include "Debug.h"

GuiCheckBox::GuiCheckBox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::CHECKBOX, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;

	focused = app->audio->LoadFx("Assets/Audio/Fx/focused.wav");
	pressed = app->audio->LoadFx("Assets/Audio/Fx/click.wav");
}

GuiCheckBox::~GuiCheckBox()
{

}

bool GuiCheckBox::Update(float dt)
{
	int scale = app->win->GetScale();

	if (state != GuiControlState::DISABLED)
	{
		// L15: DONE 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX * scale >= bounds.x && mouseX * scale <= bounds.x + bounds.w &&
			mouseY * scale >= bounds.y && mouseY * scale <= bounds.y + bounds.h) {
			
			state = GuiControlState::FOCUSED;
			app->audio->PlayFx(focused);

			if (previousState != state) {
				LOG("Change state from %d to %d",previousState,state);
				//app->audio->PlayFx(audioFxId);
			}

			// Mouse click
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN) {
				state = GuiControlState::PRESSED;
				app->audio->PlayFx(pressed);
				
				if (this->button == GuiCheckBoxType::FULLSCREEN)
				{
					checked = !checked;

					if (checked)
						SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
					else if (!checked)
						SDL_SetWindowFullscreen(app->win->window, 0);
				}

				if (this->button == GuiCheckBoxType::VSYNC)
				{
					if (app->debug->controlFPS)
					{
						checked = false;
						app->debug->controlFPS = false;
					}
					else
					{
						checked = true;
						app->debug->controlFPS = true;
					}
				}
			}
			
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




bool GuiCheckBox::Draw(Render* render)
{
	//L15: DONE 4: Draw the button according the GuiControl State
	if (active) {
		if (app->debug->colourblind) {
			switch (state)
			{
			case GuiControlState::DISABLED:
				render->DrawRectangle(bounds, 225, 225, 200, 200, checked, false);
				break;
			case GuiControlState::NORMAL:
				render->DrawRectangle(bounds, 0, 0, 255, 200, checked, false);
				break;
			case GuiControlState::FOCUSED:
				render->DrawRectangle(bounds, 153, 153, 255, 200, checked, false);
				break;
			case GuiControlState::PRESSED:
				render->DrawRectangle(bounds, 0, 255, 0, 200, checked, false);
				break;
			}
			if (this->button == GuiCheckBoxType::VSYNC) {
				if (checked) {
					app->render->DrawText("OFF", bounds.x + 60, bounds.y, 40, bounds.h, { 255,255,255 });

				}
				else {
					app->render->DrawText("ON", bounds.x + 60, bounds.y, 40, bounds.h, { 255,255,255 });
				}
			}
			else if (this->button == GuiCheckBoxType::FULLSCREEN) {
				if (checked) {
					app->render->DrawText("ON", bounds.x + 60, bounds.y, 40, bounds.h, { 255,255,255 });

				}
				else {
					app->render->DrawText("OFF", bounds.x + 60, bounds.y, 40, bounds.h, { 255,255,255 });
				}
			}
		}
		else {
			switch (state)
			{
			case GuiControlState::DISABLED:
				render->DrawRectangle(bounds, 200, 200, 200, 200, checked, false);
				break;
			case GuiControlState::NORMAL:
				render->DrawRectangle(bounds, 255, 255, 255, 200, checked, false);
				break;
			case GuiControlState::FOCUSED:
				render->DrawRectangle(bounds, 153, 153, 255, 200, checked, false);
				break;
			case GuiControlState::PRESSED:
				render->DrawRectangle(bounds, 0, 255, 0, 200, app->debug->controlFPS, false);
				break;
			}
			if (this->button == GuiCheckBoxType::VSYNC) {
				if (checked) {
					app->render->DrawText("ON", bounds.x + 60, bounds.y, 40, bounds.h, { 255,255,255 });

				}
				else {
					app->render->DrawText("OFF", bounds.x + 60, bounds.y, 40, bounds.h, { 255,255,255 });
				}
			}
			else if (this->button == GuiCheckBoxType::FULLSCREEN) {
				if (checked) {
					app->render->DrawText("ON", bounds.x + 60, bounds.y, 40, bounds.h, { 255,255,255 });

				}
				else {
					app->render->DrawText("OFF", bounds.x + 60, bounds.y, 40, bounds.h, { 255,255,255 });
				}
			}
		}
		
	}

	

	return false;
}