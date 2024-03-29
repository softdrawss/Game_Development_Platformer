#ifndef __ENEMYWALK_H__
#define __ENEMYWALK_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "PathFinding.h"

struct SDL_Texture;

class EnemyWalk : public Entity
{
public:

	EnemyWalk();
	virtual ~EnemyWalk();

	bool Awake();
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);
	void LoadAnimations();
	void SetPosition(int posX, int posY);
	bool PlayerInRange(double range);

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
	Animation idle, move;
	Animation hit, death;
	
	//FX
	int pickCoinFxId, audio;
	const char* audiopath;

	//PATHFINDING
	DynArray<iPoint> path;

};

#endif // __ENEMYWALK_H__
