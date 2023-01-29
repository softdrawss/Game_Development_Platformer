#ifndef __TITLE_H__
#define __TITLE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

struct SDL_Texture;

class Title : public Module
{
public:

	Title(bool startEnabled);

	// Destructor
	virtual ~Title();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void DrawCredits();
	void DrawSettings();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);
	bool CheckContinue();
	bool saved;

public:
	//Buttons
	GuiButton* B_play;
	GuiButton* B_continue;
	GuiButton* B_settings;
	GuiButton* B_credits;
	GuiButton* B_exit;

	//Sliders
	GuiSlider* S_music;
	GuiSlider* S_fx;

	//CheckBox
	GuiCheckBox* C_screen;
	GuiCheckBox* C_vysinc;
	bool credits = false, settings = false, exit = false;;

private:
	SDL_Texture* img;
	SDL_Texture* animImg;
	Animation anim, run;
	Animation* currentAnim = nullptr;

	char* titlepath;
	char* animpath;
	int x, y, xA, yA;
};

#endif // __TITLE_H__