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
	pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 12, bodyType::DYNAMIC);

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
	
	if (!alive)
	{
		isIdle = false;
		currentAnim = &death;
		app->entityManager->DestroyEntity(this);
	}
	else
	{
		isIdle = true;
		if (!isGrounded) {
			//Code to see if the player has approached the enemy
			/*if () {

			}*/
		}
	}

	//Here just for debbugging, to see what velocity we like for enemies
	vel.x = 0;
	vel.y = 0;
	//Set the velocity of the pbody of the enemy
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 18;

	//Animation
	/*SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, flip, &rect2);
	currentAnim->Update();*/
	return true;
}

bool EnemyFly::PostUpdate()
{
	//For highscore

	return true;
}

bool EnemyFly::CleanUp()
{
	RELEASE(texture);
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
		app->audio->PlayFx(pickCoinFxId);
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
	//right.PushBack({ 0, 160, 32, 32 });
	//right.PushBack({ 32, 160, 32, 32 });
	//right.PushBack({ 64, 160, 32, 32 });
	//right.PushBack({ 96, 160, 32, 32 });
	//right.speed = 0.08f;

	//left.PushBack({ 0, 416, 32, 32 });
	//left.PushBack({ 32, 416, 32, 32 });
	//left.PushBack({ 64, 416, 32, 32 });
	//left.PushBack({ 96, 416, 32, 32 });
	//left.speed = 0.08f;

	//// Run animation, there is also a walk animation
	//// but as we are not using anything that changes the speed I'm going to stick to walk
	//RRun.PushBack({ 0, 64, 32, 32 });
	//RRun.PushBack({ 32, 64, 32, 32 });
	//RRun.PushBack({ 64, 64, 32, 32 });
	//RRun.PushBack({ 96, 64, 32, 32 });
	//RRun.PushBack({ 128, 64, 32, 32 });
	//RRun.PushBack({ 160, 64, 32, 32 });
	//RRun.speed = 0.1f;

	//LRun.PushBack({ 0, 320, 32, 32 });
	//LRun.PushBack({ 32, 320, 32, 32 });
	//LRun.PushBack({ 64, 320, 32, 32 });
	//LRun.PushBack({ 96, 320, 32, 32 });
	//LRun.PushBack({ 128, 320, 32, 32 });
	//LRun.PushBack({ 160, 320, 32, 32 });
	//LRun.speed = 0.1f;

	//climb.PushBack({ 0, 128, 32, 32 });
	//climb.PushBack({ 32, 128, 32, 32 });
	//climb.PushBack({ 64, 128, 32, 32 });
	//climb.PushBack({ 96, 128, 32, 32 });
	//climb.speed = 0.1f;

	//RJump.PushBack({ 224, 32, 32, 32 });
	//RJump.PushBack({ 256, 32, 32, 32 });
	//RJump.PushBack({ 288, 32, 32, 32 });
	//RJump.PushBack({ 320, 32, 32, 32 });
	//RJump.PushBack({ 352, 32, 32, 32 });
	//RJump.PushBack({ 384, 32, 32, 32 });
	//RJump.PushBack({ 416, 32, 32, 32 });
	//RJump.PushBack({ 448, 32, 32, 32 });
	//RJump.speed = 0.2f;
	//RJump.loop;

	//LJump.PushBack({ 224, 288, 32, 32 });
	//LJump.PushBack({ 256, 288, 32, 32 });
	//LJump.PushBack({ 288, 288, 32, 32 });
	//LJump.PushBack({ 320, 288, 32, 32 });
	//LJump.PushBack({ 352, 288, 32, 32 });
	//LJump.PushBack({ 384, 288, 32, 32 });
	//LJump.PushBack({ 416, 288, 32, 32 });
	//LJump.PushBack({ 448, 288, 32, 32 });
	//LJump.speed = 0.2f;
	//LJump.loop;

	//death.PushBack({ 224, 128, 32, 32 });
	//death.PushBack({ 256, 128, 32, 32 });
	//death.PushBack({ 288, 128, 32, 32 });
	//death.PushBack({ 320, 128, 32, 32 });
	//death.PushBack({ 352, 128, 32, 32 });
	//death.PushBack({ 384, 128, 32, 32 });
	//death.PushBack({ 416, 128, 32, 32 });
	//death.PushBack({ 448, 128, 32, 32 });
	//death.PushBack({ 480, 128, 32, 32 });
	//death.speed = 0.1f;
	//death.loop = false;


	//currentAnim = &right;
}

void EnemyFly::SetPosition(int posX, int posY)
{
	b2Vec2 position = { PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY) };
	pbody->body->SetTransform(position, 0);
}