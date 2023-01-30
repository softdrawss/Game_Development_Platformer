#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "PlayerInteract.h"
#include "Item.h"
#include "EnemyWalk.h"
#include "EnemyFly.h"
#include "NPCChickBoy.h"
#include "Coin.h"
#include "Health.h"
#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"

#define MAX_COINS 4
#define MAX_HEATH 4


struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(bool startEnabled);

	// Destructor
	virtual ~Scene();

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

	void PauseMenu();

	void DrawSettings();

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	PlayerInteract* playerInteract;
	EnemyFly* enemyFly;
	EnemyWalk* enemyWalk;
	NPCChickBoy* npcChickBoy;
	Coin* coin;
	Health* health;

	int camSpeed;

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	SDL_Rect rectCamera;
	bool continue_pressed;

	//Triggers
	bool door_A;
	bool trapDoor_A;
	
	bool pause = false;
	GuiButton* B_resume;
	GuiButton* B_settings;
	GuiButton* B_back_to_title;
	GuiButton* B_exit;
	GuiButton* B_back;
	
	//Sliders
	GuiSlider* S_music;
	GuiSlider* S_fx;

	//CheckBox
	GuiCheckBox* C_screen;
	GuiCheckBox* C_vysinc;
	bool credits = false, settings = false, exit = false;
private:
	SDL_Texture* img;

	//Music XML
	char* musicPath;

	//Fonts XML
	char* fontPath;

	//CAMERA XML
	int camX, camY, speed;
	int coins_value;

	//Count
	int coinCount;
	int healthCount;
	int coinPicked;

	//sfx
	char* audioPath;
	int pickCoinAudio_sfx;

	//Anim
	Animation coinUI, healthUI;
	Animation* coinUIanim;
	Animation* healthUIanim;	

	//Textures
	const char* coinPath;
	const char* healthPath;
	SDL_Texture* coinText;
	SDL_Texture* healthText;
};

#endif // __SCENE_H__