#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Animation.h"
#include "Point.h"
#include "Physics.h"


struct Collider;

struct Particles
{
public:
	// Constructor
	Particles(bool startEnabled);

	// Copy constructor
	Particles(const Particles& p);

	// Destructor
	~Particles();

	// Called in ModuleParticles' Update
	// Handles the logic of the particle
	// Returns false when the particle reaches its lifetime
	bool Update();

	// Sets flag for deletion and for the collider aswell
	void SetToDelete();

public:
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
	Collider* collider = nullptr;

	// A flag for the particle removal. Important! We do not delete objects instantly
	bool pendingToDelete = false;

	bool hasExplosion;
};

#endif //__PARTICLE_H__#pragma once
