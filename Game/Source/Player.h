#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
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

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void LoadAnimations();

public:

	bool alive;
	b2Vec2 initPosition;
	//Physics
	PhysBody* pbody;



private:
	
	bool idle, leftID;
	bool isGrounded;
	bool stairs;
	int remainingJumpSteps;

	//Texture
	SDL_Texture* texture;
	const char* texturePath;

	//Animation
	Animation left, right, climb;	// jump?
	Animation LRun, RRun, LJump, RJump;
	Animation death, iddledeathanim;
	Animation* currentAnim = nullptr;

	//FX
	int pickCoinFxId;
	
};

#endif // __PLAYER_H__