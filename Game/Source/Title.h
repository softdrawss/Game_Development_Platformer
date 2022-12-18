#ifndef __TITLE_H__
#define __TITLE_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"

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

public:

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