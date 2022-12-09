#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Animation.h"
#include "Point.h"
#include "Physics.h"
#include "Log.h"
struct Particles : public Module
{
public:
	// Constructor
	Particles(bool startEnabled);

	// Copy constructor
	Particles(const Particles& p);

	// Destructor
	~Particles();

	bool Start();

	bool PreUpdate();

	// Called in ModuleParticles' Update
	// Handles the logic of the particle
	// Returns false when the particle reaches its lifetime
	bool Update();

	bool PostUpdate();

	bool CleanUp();
	
	// Sets flag for deletion and for the collider aswell
	void SetToDelete();
	void LoadAnimations();

	void OnCollision(PhysBody* physA, PhysBody* physB) override;

	//	Particle* AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType = Collider::Type::NONE, uint delay = 0);

public:
	SDL_Texture* texture = nullptr;
	
	// Defines the position in the screen
	b2Vec2 position;

	// Defines the speed at which the particle will move (pixels per second)
	fPoint speed;

	// A set of rectangle sprites
	Animation anim;

	// Defines wether the particle is alive or not
	// Particles will be set to not alive until "spawnTime" is reached
	bool isAlive = false;

	// Defines the amout of frames this particle has been active
	// Negative values mean the particle is waiting to be activated
	int frameCount = 0;

	// Defines the total amount of frames during which the particle will be active
	uint lifetime = 0;

	// The particle's collider
	PhysBody* pbody;

	// A flag for the particle removal. Important! We do not delete objects instantly
	bool pendingToDelete = false;

	bool hasExplosion;
};

#endif //__PARTICLE_H__#pragma once
