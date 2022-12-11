#include "EnemyWalk.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Debug.h"
#include "FadeToBlack.h"
#include "EntityManager.h"

EnemyWalk::EnemyWalk() : Entity(EntityType::WALK)
{
	name.Create("walk");
}

EnemyWalk::~EnemyWalk() {

}

bool EnemyWalk::Awake() {

	//L02: DONE 5: Get Player parameters from XML

	return true;
}

bool EnemyWalk::Start()
{
	alive = true;

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	initPosition.x = position.x;
	initPosition.y = position.y;

	//initilize textures
	texturePath = parameters.attribute("texturepath").as_string();
	texture = app->tex->Load(texturePath);

	isAsleep = true;

	// L07 DONE 5: Add physics to the enemy - initialize physics body
	pbody = app->physics->CreateRectangle(position.x, position.y, 11, 18, bodyType::DYNAMIC);

	//// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	LoadAnimations();

	return true;
}

bool EnemyWalk::Update()
{
	b2Vec2 vel;
	int speed = 4;

	vel = pbody->body->GetLinearVelocity() + b2Vec2(0, -GRAVITY_Y * 0.0166);


	//Death
	if (!alive)
	{
		pbody->body->SetActive(false);
		pbody->body->SetType(b2_staticBody);
		isAsleep = false;
		currentAnim = &death;
	}
	else
	{
		isAsleep = true;

		//Left
		if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
			currentAnim = &move;
			vel.x = -speed;
			isAsleep = false;
			flip = SDL_FLIP_HORIZONTAL;
		}
		//Right
		else if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
			currentAnim = &move;
			vel.x = speed;
			isAsleep = false;
			flip = SDL_FLIP_NONE;
		}
		else
			vel.x = 0;

		//Start charge
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT && isCharging == 0 && !isShooting)
		{
			isCharging = 120;
			currentAnim = &shoot;
			isAsleep = false;
		}

		//Charging
		if (isCharging == 1)
		{
			isCharging = 0;
			isShooting = true;
		}
		else if (isCharging != 0)
		{
			if (app->input->GetKey(SDL_SCANCODE_E) == KEY_UP)
			{
				isCharging = 0;
				isAsleep = true;
			}
			else
			{
				isCharging--;
				isAsleep = false;
			}
		}
	}

	//Set the velocity of the pbody of the enemy
	pbody->body->SetLinearVelocity(vel);

	//Set Enemy position
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 28;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 19;

	//Animations
	if (isShooting && shoot.GetCurrentFrameint() == 3)
	{
		shoot.Reset();
		isShooting = false;
	}

	if (isAsleep) { currentAnim = &sleep; }
	if (isCharging != 0) { currentAnim = &charge; }
	if (isShooting) { currentAnim = &shoot; }
	SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, flip, &rect2);
	currentAnim->Update();

	return true;
}

bool EnemyWalk::PostUpdate()
{
	//For highscore
	
	return true;
}

bool EnemyWalk::CleanUp()
{
	app->entityManager->DestroyEntity(this);
	//RELEASE(texture);
	return true;
}

void EnemyWalk::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::DEATH:
		LOG("Collision DEATH");
		//alive = false;
		break;
	case ColliderType::GROUND:
		LOG("Collision GROUND");
		isGrounded = true;
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		isGrounded = true;
		break;
	case ColliderType::PLAYER:
		LOG("Collision PLATFORM");
		alive = false;
		break;
	case ColliderType::SHOT:
		LOG("Collision SHOT");
		alive = false;
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}

void EnemyWalk::LoadAnimations()
{
	sleep.PushBack({ 0, 0, 55, 30 });

	wake.PushBack({   0, 30, 55, 30 });
	wake.PushBack({  55, 30, 55, 30 });
	wake.PushBack({ 110, 30, 55, 30 });
	wake.PushBack({ 165, 30, 55, 30 });
	wake.PushBack({ 220, 30, 55, 30 });
	wake.loop = false;
	wake.speed = 0.1f;

	move.PushBack({   0, 60, 55, 30 });
	move.PushBack({  55, 60, 55, 30 });
	move.PushBack({ 110, 60, 55, 30 });
	move.PushBack({ 165, 60, 55, 30 });
	move.PushBack({ 220, 60, 55, 30 });
	move.PushBack({ 275, 60, 55, 30 });
	move.PushBack({ 330, 60, 55, 30 });
	move.PushBack({ 385, 60, 55, 30 });
	move.speed = 0.2f;

	dash.PushBack({ 385,   0, 120, 30 });
	dash.PushBack({ 385,  30, 120, 30 });
	dash.PushBack({ 385,  60, 120, 30 });
	dash.PushBack({ 385,  90, 120, 30 });
	dash.PushBack({ 385, 120, 120, 30 });
	dash.PushBack({ 385, 150, 120, 30 });
	dash.PushBack({ 385, 180, 120, 30 });
	dash.loop = false;
	dash.speed = 0.4f;

	charge.PushBack({   0, 90, 55, 30 });
	charge.PushBack({  55, 90, 55, 30 });
	charge.PushBack({ 110, 90, 55, 30 });
	charge.PushBack({ 165, 90, 55, 30 });
	charge.speed = 0.2f;
	 
	shoot.PushBack({   0, 120, 55, 30 });
	shoot.PushBack({  55, 120, 55, 30 });
	shoot.PushBack({ 110, 120, 55, 30 });
	shoot.PushBack({ 165, 120, 55, 30 });
	shoot.loop = false;
	shoot.speed = 0.3f;

	hit.PushBack({  0, 150, 55, 30 });
	hit.PushBack({ 55, 150, 55, 30 });
	hit.loop = false;
	hit.speed = 0.2f;

	death.PushBack({   0, 180, 55, 30 });
	death.PushBack({  55, 180, 55, 30 });
	death.PushBack({ 110, 180, 55, 30 });
	death.PushBack({ 165, 180, 55, 30 });
	death.PushBack({ 220, 180, 55, 30 });
	death.PushBack({ 275, 180, 55, 30 });
	death.loop = false;
	death.speed = 0.2f;

	currentAnim = &sleep;
}

void EnemyWalk::SetPosition(int posX, int posY)
{
	b2Vec2 position = { PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY) };
	pbody->body->SetTransform(position, 0);
}