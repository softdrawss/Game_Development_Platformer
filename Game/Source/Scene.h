#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "EnemyWalk.h"
#include "EnemyFly.h"

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

public:

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	EnemyFly* enemyFly;
	EnemyWalk* enemyWalk;

	int camSpeed;

	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

	SDL_Rect rectCamera;

private:
	SDL_Texture* img;

	//Music XML
	char* musicPath;

	//Fonts XML
	char* fontPath;

	//CAMERA XML
	int camX, camY, speed;
};

#endif // __SCENE_H__