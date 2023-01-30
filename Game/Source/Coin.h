#ifndef __COIN_H__
#define __COIN_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"

struct SDL_Texture;

class Coin : public Entity
{
public:

	Coin();
	virtual ~Coin();

	bool Awake();
	bool Start();
	bool Update(float dt);	
	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void SetPosition(int posX, int posY);
	bool LookingRight();
	void LoadAnimations();

	bool CheckPickingCoin();
public:

	bool alive, isGrounded;
	b2Vec2 initPosition;
	//Physics
	PhysBody* pbody;

private:
	bool isPicked = false;

	//Bools

	//Texture
	SDL_Texture* texture;
	const char* texturePath;

	//Animation
	Animation* currentAnim = nullptr;
	SDL_RendererFlip flip;
	Animation rot;

};

#endif // __PLAYER_H__