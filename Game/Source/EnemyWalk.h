#ifndef __ENEMYWALK_H__
#define __ENEMYWALK_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class EnemyWalk : public Entity
{
public:

	EnemyWalk();
	virtual ~EnemyWalk();

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

	bool isIdle, flipLeft;
	bool isGrounded;
	bool stairs;
	int remainingJumpSteps;

	//Texture
	SDL_Texture* texture;
	const char* texturePath;

	//Animation
	Animation idle, walk;	// jump?
	Animation attack;
	Animation death, iddledeathanim;
	Animation* currentAnim = nullptr;
	SDL_RendererFlip flip;

	//FX
	int pickCoinFxId;

};

#endif // __ENEMYWALK_H__
