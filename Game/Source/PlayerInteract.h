#ifndef __PLAYERINTERACT_H__
#define __PLAYERINTERACT_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"

struct SDL_Texture;

class PlayerInteract : public Entity
{
public:

	PlayerInteract();
	virtual ~PlayerInteract();

	bool Awake();

	bool Start();

	bool Update();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

public:

	bool isActive = false;
	//b2Vec2 position;

private:

	PhysBody* pbody;
};

#endif // __PLAYERINTERACT_H__