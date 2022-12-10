#include "App.h"
#include "Particles.h"
#include "Textures.h"
#include "Render.h"

ParticleBody::ParticleBody()
{
	position.x = 0;
	position.y = 0;
	speed.SetToZero();
}

ParticleBody::ParticleBody(const ParticleBody& p) : anim(p.anim), position(p.position), speed(p.speed),
frameCount(p.frameCount), lifetime(p.lifetime), hasExplosion(p.hasExplosion)
{

}

ParticleBody::~ParticleBody()
{
	
}

bool ParticleBody::Update()
{
	bool ret = true;
	frameCount++;

	// The particle is set to 'alive' when the delay has been reached
	if (!isAlive && frameCount >= 0)
		isAlive = true;

	if (isAlive)
	{
		anim.Update();

		// If the particle has a specific lifetime, check when it has to be destroyed
		if (lifetime > 0)
		{
			if (frameCount >= lifetime)
				ret = false;
		}
		// Otherwise the particle is destroyed when the animation is finished
		else if (anim.HasFinished())
			ret = false;

		// Update the position in the screen
		position.x += speed.x;
		position.y += speed.y;

		if (pbody != nullptr) {
			/*if (isexplosion == true) {
				collider->SetPos(position.x - 10, position.y - 5);
			}
			else {
				collider->SetPos(position.x, position.y);
			}*/
		}

	}

	return ret;
}

void ParticleBody::SetToDelete()
{
	//pendingToDelete = true;
	if (pbody != nullptr)
		RELEASE(pbody);
}

Particles::Particles(bool startEnabled) : Module(startEnabled)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;
}

Particles::~Particles()
{

}

bool Particles::Start()
{
	LOG("Loading particles");
	textureShot = app->tex->Load("Assets/Textures/Characters/1_Pink_Monster/Rock1.png");
	//LoadAnimations();

	shot.anim.PushBack({ 0, 0, 32, 32 });
	
	shot.anim.speed = 0.08f;
	shot.lifetime = 18;
	shot.speed.x = 2;
	return true;
}

bool Particles::PreUpdate()
{
	// Remove all particles scheduled for deletion
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr && particles[i]->pendingToDelete)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}

bool Particles::Update(float dt)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		ParticleBody* particle = particles[i];

		if (particle == nullptr) {
			continue;
		}

		// Call particle Update. If it has reached its lifetime, destroy it
		if (particle->Update() == false)
		{
			if (particle->hasExplosion == true) {
			}
			particles[i]->SetToDelete();
		}
	}
	return true;
}

bool Particles::PostUpdate()
{
	//Iterating all particle array and drawing any active particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		ParticleBody* particle = particles[i];

		if (particle != nullptr && particle->isAlive)
		{
			app->render->DrawTexture(textureShot, particle->position.x - 25, particle->position.y - 25, SDL_FLIP_NONE, &(particle->anim.GetCurrentFrame()));
		}
	}

	return true;
}

bool Particles::CleanUp()
{
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	for (int i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}

void Particles::OnCollision(PhysBody* c1, PhysBody* c2)
{
	//for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	//{
	//	// Always destroy particles that collide
	//	if (particles[i] != nullptr && particles[i]->collider == c1)
	//	{
	//		particles[i]->pendingToDelete = true;
	//		particles[i]->collider->pendingToDelete = true;
	//		break;
	//	}
	//}

	switch (c2->ctype)
	{
	case ColliderType::ENEMY:
		break;
	case ColliderType::GROUND:
		LOG("Collision GROUND");
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}

ParticleBody* Particles::AddParticle(const ParticleBody& particle, int x, int y, ColliderType type, uint delay)
{
	ParticleBody* newParticle = nullptr;

	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		//Finding an empty slot for a new particle
		if (particles[i] == nullptr)
		{
			newParticle = new ParticleBody(particle);
			newParticle->frameCount = -(int)delay;			// We start the frameCount as the negative delay
			newParticle->position.x = x;						// so when frameCount reaches 0 the particle will be activated
			newParticle->position.y = y;

			//Adding the particle's collider
			if (type != ColliderType::UNKNOWN) {
				newParticle->pbody = app->physics->CreateCircle(newParticle->position.x+24, newParticle->position.y+12, 5, bodyType::DYNAMIC);
				newParticle->pbody->ctype = ColliderType::SHOT;
				//Update();
				//app->render->DrawTexture(texture, newParticle->position.x - 25, newParticle->position.y - 25, SDL_FLIP_NONE, &(newParticle->anim.GetCurrentFrame()));

			}
			particles[i] = newParticle;
			break;
		}
	}

	return newParticle;
}

void Particles::LoadAnimations() {
	shot.anim.PushBack({ 32, 0, 32, 32 });
	shot.anim.PushBack({ 64, 0, 32, 32 });
	shot.anim.speed = 0.08f;
	shot.lifetime = 18;
	shot.speed.x = 2;
}