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
	void SetPosition(int posX, int posY);
	void Attack();
	bool LookingRight();
	void LoadAnimations();

public:

	bool alive;
	b2Vec2 initPosition;
	//Physics
	PhysBody* pbody;

private:
	
	//Bools
	bool isIdle;
	bool isGrounded;
	bool wallJumpRight, wallJumpLeft;
	bool stairs;
	int remainingJumpSteps;
	bool isAttacking;

	//Texture
	SDL_Texture* texture;
	const char* texturePath;

	//Animation
	Animation* currentAnim = nullptr;
	SDL_RendererFlip flip;
	Animation idle, run, jump, climb;
	Animation attackNormal;
	Animation death;

	//FX
	int pickCoinFxIdaudio, jumpaudio, deathaudio;
	const char* jumppath;
	const char* deathpath;
};

#endif // __PLAYER_H__