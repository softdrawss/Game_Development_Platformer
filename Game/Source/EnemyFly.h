#ifndef __ENEMYFLY_H__
#define __ENEMYFLY_H__

#include "Entity.h"
#include "Point.h"
#include "Physics.h"
#include "SDL/include/SDL.h"
#include "Animation.h"
#include "PathFinding.h"

struct SDL_Texture;

class EnemyFly : public Entity
{
public:

	EnemyFly();
	virtual ~EnemyFly();

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

	bool isIdle, flipLeft;
	bool isGrounded;
	bool stairs;
	int remainingJumpSteps;

	//Texture
	SDL_Texture* texture;
	const char* texturePath;

	//Animation
	Animation idle, run, fly;
	Animation death, iddledeathanim;
	Animation* currentAnim = nullptr;
	SDL_RendererFlip flip;

	//FX
	int pickCoinFxId, audio;
	const char* audiopath;

	//PATHFINDING
	DynArray<iPoint> path;
	iPoint lastEnemyTile;

};

#endif // __ENEMYFLY_H__
