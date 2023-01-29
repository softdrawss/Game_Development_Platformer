#ifndef __HEALTH_H__
#define __HEALTH_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Health : public Entity
{
public:

	Health();
	virtual ~Health();

	bool Awake();
	bool Start();
	bool Update();	
	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void SetPosition(int posX, int posY);
	bool LookingRight();
	void LoadAnimations();

	bool CheckPickingHealth();

public:

	bool alive;
	b2Vec2 initPosition;
	//Physics
	PhysBody* pbody;

private:
	bool isPicked = false;

	//Bools

	//Texture
	SDL_Texture* texture;
	const char* texturePath;
	const char* audioPath;

	//Animation
	Animation* currentAnim = nullptr;
	SDL_RendererFlip flip;
	Animation rot;

	//FX
	int pickCoinAudio;
};

#endif // __HEALTH_H__