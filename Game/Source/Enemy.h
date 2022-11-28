#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Enemy : public Entity
{
public:

	Enemy();
	virtual ~Enemy();

	bool Awake();
	bool Start();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void LoadAnimations();
	void SetPosition(int posX, int posY);

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

#endif // __ENEMY_H__
