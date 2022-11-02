#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Player : public Entity
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update();
	
	bool PostUpdate();

	bool CleanUp();

	bool IsJumping();

public:

private:
	
	//L02: DONE 1: Declare player parameters
	SDL_Texture* texture;
	const char* texturePath;

	int remainingJumpSteps;
	// L07 DONE 5: Add physics to the player - declare a Physics body
	PhysBody* pbody;
	Animation left, right, climb;	// jump?
	Animation LRun, RRun, LJump, RJump;
	Animation death, iddledeathanim;

	Animation* currentAnim = nullptr;
	bool iddle;
};

#endif // __PLAYER_H__