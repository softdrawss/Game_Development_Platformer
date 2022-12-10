#include "Particles.h"

Particles::Particles(bool startEnabled):Module(startEnabled)
{
	//position.SetToZero();
	speed.SetToZero();
}

Particles::Particles(const Particles& p) : anim(p.anim), position(p.position), speed(p.speed),
frameCount(p.frameCount), lifetime(p.lifetime), hasExplosion(p.hasExplosion)
{

}

Particles::~Particles()
{
	//if (collider != nullptr) {}
	//	collider->pendingToDelete = true;
}

bool Particles::Start()
{
	bool ret = true;
	return ret;
}

bool Particles::PreUpdate()
{
	bool ret = true;
	return ret;
}

bool Particles::Update()
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

bool Particles::PostUpdate()
{
	bool ret = true;
	return ret;
}

bool Particles::CleanUp()
{
	bool ret = true;
	return ret;
}

void Particles::SetToDelete()
{
	pendingToDelete = true;
	if (pbody != nullptr) {}
		//collider->pendingToDelete = true;
}

void Particles::LoadAnimations() {

}

void Particles::OnCollision(PhysBody* physA, PhysBody* physB) {
	switch (physB->ctype)
	{
	case ColliderType::DEATH:
		LOG("Collision DEATH");
		//alive = false;
		break;
	case ColliderType::GROUND:
		LOG("Collision GROUND");
		//isGrounded = true;
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		//app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		//isGrounded = true;
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}