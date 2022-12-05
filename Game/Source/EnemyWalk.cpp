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

	texturePath = parameters.attribute("texturepath").as_string();
	//initilize textures
	texture = app->tex->Load(texturePath);

	isIdle = true;

	// L07 DONE 5: Add physics to the enemy - initialize physics body
	//We have to or we will have to change the radium, since probably the enemy is not as big as the player
	pbody = app->physics->CreateCircle(position.x + 10, position.y + 10, 12, bodyType::DYNAMIC);

	//// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;
	pbody->ctype = ColliderType::ENEMY;

	LoadAnimations();

	return true;
}

bool EnemyWalk::Update()
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
		if (isGrounded) {
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

	//Not rlly necessary? But I will leave it to use it if we want enemies to jump, even though I think they are probably not capable because of the level
	//Apply Jump Force
	//if (remainingJumpSteps > 0)
	//{
	//	float force = pbody->body->GetMass() * 10 / 0.01666; //F = mv/t (t = 1/60fps)
	//	force /= 6.0;
	//	pbody->body->ApplyForce(b2Vec2(0, -force), pbody->body->GetWorldCenter(), true);
	//	remainingJumpSteps--;
	//}

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 18;

	//Animations
	SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, SDL_FLIP_NONE, &rect2);
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
	app->tex->UnLoad(texture);
	RELEASE(texture);
	return true;
}

void EnemyWalk::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::DEATH:
		LOG("Collision DEATH");
		alive = false;
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
	idle.PushBack({ 15, 0, 20, 26 });
	idle.PushBack({ 15, 26, 20, 26 });
	idle.PushBack({ 15, 52, 20, 26 });
	idle.PushBack({ 15, 78, 20, 26 });
	idle.PushBack({ 15, 104, 20, 26 });
	idle.speed = 0.08f;

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


	currentAnim = &idle;
}

void EnemyWalk::SetPosition(int posX, int posY)
{
	b2Vec2 position = { PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY) };
	pbody->body->SetTransform(position, 0);
}