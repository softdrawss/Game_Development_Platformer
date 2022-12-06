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
	pbody = app->physics->CreateRectangle(position.x, position.y, 11, 22, bodyType::DYNAMIC);

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
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT && isCharging != 0) {
			isCharging = 120;
			currentAnim = &shoot;
			isAsleep = false;
		}

		//Charging
		if (isCharging != 0)
		{
			isCharging--;
			isAsleep = false;
		}
		else if (app->input->GetKey(SDL_SCANCODE_E) == KEY_UP)
		{
			isCharging = 0;
			isAsleep = true;
		}
	}

	//Set the velocity of the pbody of the enemy
	pbody->body->SetLinearVelocity(vel);

	//Set Enemy position
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x);
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y);

	////Animations
	//if (isAttackingRock && attackRock.GetCurrentFrameint() == 4)
	//{
	//	attackRock.Reset();
	//	isAttackingRock = false;
	//}

	/*if (isAsleep && !isAttackingRock) { currentAnim = &sleep; }
	if (isAttackingRock) { currentAnim = &attackRock; }*/
	SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x - 14, position.y - 7, flip, &rect2);
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
	RELEASE(texture);
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
	sleep.PushBack({ 0, 0, 50, 30 });

	wake.PushBack({   0, 30, 50, 30 });
	wake.PushBack({  50, 30, 50, 30 });
	wake.PushBack({ 100, 30, 50, 30 });
	wake.PushBack({ 150, 30, 50, 30 });
	wake.PushBack({ 200, 30, 50, 30 });
	wake.loop = false;
	wake.speed = 0.1f;

	move.PushBack({   0, 60, 50, 30 });
	move.PushBack({  50, 60, 50, 30 });
	move.PushBack({ 100, 60, 50, 30 });
	move.PushBack({ 150, 60, 50, 30 });
	move.PushBack({ 200, 60, 50, 30 });
	move.PushBack({ 250, 60, 50, 30 });
	move.PushBack({ 300, 60, 50, 30 });
	move.PushBack({ 250, 60, 50, 30 });
	move.speed = 0.1f;

	dash.PushBack({ 300,   0, 115, 30 });
	dash.PushBack({ 300,  30, 115, 30 });
	dash.PushBack({ 300,  60, 115, 30 });
	dash.PushBack({ 300,  90, 115, 30 });
	dash.PushBack({ 300, 120, 115, 30 });
	dash.PushBack({ 300, 150, 115, 30 });
	dash.PushBack({ 300, 180, 115, 30 });
	dash.loop = false;
	dash.speed = 0.1f;

	charge.PushBack({   0, 90, 50, 30 });
	charge.PushBack({  50, 90, 50, 30 });
	charge.PushBack({ 100, 90, 50, 30 });
	charge.PushBack({ 150, 90, 50, 30 });
	charge.speed = 0.1f;
	 
	shoot.PushBack({   0, 120, 50, 30 });
	shoot.PushBack({  50, 120, 50, 30 });
	shoot.PushBack({ 100, 120, 50, 30 });
	shoot.PushBack({ 150, 120, 50, 30 });
	shoot.loop = false;
	shoot.speed = 0.1f;

	hit.PushBack({  0, 150, 50, 30 });
	hit.PushBack({ 50, 150, 50, 30 });
	hit.loop = false;
	hit.speed = 0.1f;

	death.PushBack({   0, 180, 50, 30 });
	death.PushBack({  50, 180, 50, 30 });
	death.PushBack({ 100, 180, 50, 30 });
	death.PushBack({ 150, 180, 50, 30 });
	death.PushBack({ 200, 180, 50, 30 });
	death.PushBack({ 250, 180, 50, 30 });
	death.loop = false;
	death.speed = 0.1f;

	currentAnim = &sleep;
}

void EnemyWalk::SetPosition(int posX, int posY)
{
	b2Vec2 position = { PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY) };
	pbody->body->SetTransform(position, 0);
}