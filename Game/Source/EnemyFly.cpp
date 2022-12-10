#include "EnemyFly.h"
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
#include "Pathfinding.h"
#include "EntityManager.h"

EnemyFly::EnemyFly() : Entity(EntityType::FLY)
{
	name.Create("fly");
}

EnemyFly::~EnemyFly() {

}

bool EnemyFly::Awake() {

	//L02: DONE 1: Initialize Player parameters

	//L02: DONE 5: Get Player parameters from XML

	return true;
}

bool EnemyFly::Start()
{
	alive = true;
	
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	initPosition.x = position.x;
	initPosition.y = position.y;

	texturePath = parameters.attribute("texturepath").as_string();

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateRectangle(position.x, position.y, 15, 18, bodyType::STATIC);


	//// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	//pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	LoadAnimations();

	return true;
}

bool EnemyFly::Update()
{
	b2Vec2 vel;
	int speed = 4;

	vel = pbody->body->GetLinearVelocity() + b2Vec2(0, -GRAVITY_Y * 0.0166);
	
	if (!alive)
	{
		isIdle = false;
		currentAnim = &death;
		app->entityManager->DestroyEntity(this);
	}
	else
	{
		isIdle = true;
		
		//Code to see if the player has approached the enemy
		/*if () {

		}*/

		//Left
		if (app->input->GetKey(SDL_SCANCODE_J) == KEY_REPEAT) {
			vel.x = -speed;
			flip = SDL_FLIP_HORIZONTAL;
		}
		//Right
		else if (app->input->GetKey(SDL_SCANCODE_K) == KEY_REPEAT) {
			vel.x = speed;
			flip = SDL_FLIP_NONE;
		}
		else
			vel.x = 0;
		
	}

	//Set the velocity of the pbody of the enemy
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 31;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 22;

	//Animation
	SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, flip, &rect2);
	currentAnim->Update();
	return true;
}

bool EnemyFly::PostUpdate()
{
	//For highscore

	return true;
}

bool EnemyFly::CleanUp()
{
	app->entityManager->DestroyEntity(this);
	return true;
}

void EnemyFly::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::DEATH:
		LOG("Collision DEATH");
		alive = false;
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

void EnemyFly::LoadAnimations()
{
	idle.PushBack({   0, 0, 64, 32 });
	idle.PushBack({  64, 0, 64, 32 });
	idle.PushBack({ 128, 0, 64, 32 });
	idle.PushBack({ 192, 0, 64, 32 });
	idle.speed = 0.08f;

	run.PushBack({   0, 32, 64, 32 });
	run.PushBack({  64, 32, 64, 32 });
	run.PushBack({ 128, 32, 64, 32 });
	run.PushBack({ 192, 32, 64, 32 });
	run.PushBack({ 224, 32, 64, 32 });
	run.speed = 0.2f;

	fly.PushBack({   0, 64, 64, 32 });
	fly.PushBack({  64, 64, 64, 32 });
	fly.PushBack({ 128, 64, 64, 32 });
	fly.PushBack({ 192, 64, 64, 32 });
	fly.speed = 0.2f;

	currentAnim = &fly;
}

void EnemyFly::SetPosition(int posX, int posY)
{
	b2Vec2 position = { PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY) };
	pbody->body->SetTransform(position, 0);
}