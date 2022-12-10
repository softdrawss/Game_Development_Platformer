#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "Animation.h"
#include "Point.h"
#include "Physics.h"
#include "Log.h"

#define MAX_ACTIVE_PARTICLES 5

struct ParticleBody
{
public:
	// Constructor
	ParticleBody();

	// Copy constructor
	ParticleBody(const ParticleBody& p);

	// Destructor
	~ParticleBody();

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
	bool isAlive = true;

	// Defines the amout of frames this particle has been active
	// Negative values mean the particle is waiting to be activated
	int frameCount = 0;

	// Defines the total amount of frames during which the particle will be active
	uint lifetime = 0;

	// The particle's collider
	PhysBody* pbody;

	// A flag for the particle removal. Important! We do not delete objects instantly
	bool pendingToDelete = false;

	bool hasExplosion = false;
};


struct Particles : public Module{
	// Constructor
	// Initializes all the particles in the array to nullptr
	Particles(bool startEnabled);

	//Destructor
	~Particles();

	// Called when the module is activated
	// Loads the necessary textures for the particles
	bool Start();

	// Called at the beginning of the application loop
	// Removes all particles pending to delete
	bool PreUpdate();

	// Called at the middle of the application loop
	// Iterates all the particles and calls its Update()
	// Removes any "dead" particles
	bool Update();

	// Called at the end of the application loop
	// Iterates all the particles and draws them
	bool PostUpdate();

	// Called on application exit
	// Destroys all active particles left in the array
	bool CleanUp();

	// Called when a particle collider hits another collider
	void OnCollision(PhysBody* c1, PhysBody* c2) override;

	// Creates a new particle and adds it to the array
	// Param particle	- A template particle from which the new particle will be created
	// Param x, y		- Position x,y in the screen (upper left axis)
	// Param delay		- Delay time from the moment the function is called until the particle is displayed in screen
	ParticleBody* AddParticle(const ParticleBody& particle, int x, int y, ColliderType type, uint delay = 0);
	
	void LoadAnimations();

private:
	// Particles spritesheet loaded into an SDL Texture
	SDL_Texture* textureShot = nullptr;

	// An array to store and handle all the particles
	ParticleBody* particles[MAX_ACTIVE_PARTICLES] = { nullptr };

public:
	ParticleBody shot;
	SDL_RendererFlip flip;

};
#endif //__PARTICLE_H__#pragma once
