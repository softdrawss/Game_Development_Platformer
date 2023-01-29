#ifndef __NPCCHICKBOY_H__
#define __NPCCHICKBOY_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class NPCChickBoy : public Entity
{
public:

	NPCChickBoy();
	virtual ~NPCChickBoy();

	bool Awake();
	bool Start();
	bool Update(float dt);
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

	bool isAsleep;
	bool isGrounded;
	int isCharging;
	bool isShooting;
	int remainingJumpSteps;

	//Texture
	SDL_Texture* texture;
	const char* texturePath;

	//Animation
	Animation* currentAnim = nullptr;
	SDL_RendererFlip flip;
	Animation idle, run;
	Animation charge, shoot;
	Animation hit, death;

	//FX
	int pickCoinFxId;

};

#endif // __NPCCHICKBOY_H__