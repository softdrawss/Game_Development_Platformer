#include "Enemy.h"
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

Enemy::Enemy() : Entity(EntityType::ENEMY)
{
	name.Create("Enemy");
}

Enemy::~Enemy() {

}

bool Enemy::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML

	return true;
}

bool Enemy::Start()
{
	//alive = true;
	//stairs = false;
	//position.x = parameters.attribute("x").as_int();
	//position.y = parameters.attribute("y").as_int();

	//initPosition.x = position.x;
	//initPosition.y = position.y;

	//texturePath = parameters.attribute("texturepath").as_string();

	////initilize textures
	//texture = app->tex->Load(texturePath);

	//remainingJumpSteps = 0;
	//idle = true;

	////id = app->tex->LoadSprite(texturePath, 15, 8);

	//// L07 DONE 5: Add physics to the player - initialize physics body
	//pbody = app->physics->CreateCircle(position.x + 16, position.y + 16, 12, bodyType::DYNAMIC);

	//// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	//pbody->listener = this;
	//pbody->ctype = ColliderType::PLAYER;

	////initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	//pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	//LoadAnimations();

	return true;
}

bool Enemy::Update()
{
	//b2Vec2 vel;
	//int speed = 5;

	//if (app->debug->godMode)
	//{
	//	alive = true;
	//	pbody->body->SetGravityScale(0);
	//}
	//else
	//{
	//	pbody->body->SetGravityScale(1);
	//	vel = pbody->body->GetLinearVelocity() + b2Vec2(0, -GRAVITY_Y * 0.0166);
	//}

	//if (!alive)
	//{
	//	idle = false;
	//	currentAnim = &death;
	//}
	//else
	//{
	//	idle = true;

	//	if (stairs || app->debug->godMode)
	//	{
	//		//Up
	//		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
	//			currentAnim = &climb;
	//			vel = b2Vec2(0, -speed);
	//			idle = false;
	//		}
	//		//Down
	//		else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
	//			currentAnim = &climb;
	//			vel = b2Vec2(0, speed);
	//			idle = false;
	//		}
	//		else
	//			vel.y = 0;
	//	}

	//	//Left
	//	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
	//		currentAnim = &LRun;
	//		vel.x = -speed;
	//		idle = false;
	//		leftID = true;
	//	}
	//	//Right
	//	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
	//		currentAnim = &RRun;
	//		vel.x = speed;
	//		idle = false;
	//		leftID = false;
	//	}
	//	else
	//		vel.x = 0;

	//	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isGrounded && remainingJumpSteps == 0) {
	//		currentAnim = leftID ? &LJump : &RJump;
	//		remainingJumpSteps = 6;
	//		idle = false;
	//		isGrounded = false;
	//	}
	//}


	////Set the velocity of the pbody of the player
	//pbody->body->SetLinearVelocity(vel);

	////Apply Jump Force
	//if (remainingJumpSteps > 0)
	//{
	//	float force = pbody->body->GetMass() * 10 / 0.01666; //F = mv/t (t = 1/60fps)
	//	force /= 6.0;
	//	pbody->body->ApplyForce(b2Vec2(0, -force), pbody->body->GetWorldCenter(), true);
	//	remainingJumpSteps--;
	//}

	////Update player position in pixelspl
	//position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	//position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 18;

	////Animations
	//if (idle) { currentAnim = leftID ? &left : &right; }
	//if (!isGrounded) { currentAnim = leftID ? &LJump : &RJump; }
	//SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	//app->render->DrawTexture(texture, position.x, position.y, &rect2);
	//currentAnim->Update();

	return true;
}

bool Enemy::PostUpdate()
{
	//For highscore

	return true;
}

bool Enemy::CleanUp()
{

	return true;
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::GROUND:
		LOG("Collision GROUND");
		isGrounded = true;
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		isGrounded = true;
		break;
	case ColliderType::DEATH:
		LOG("Collision DEATH");
		alive = false;
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}

void Enemy::LoadAnimations()
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

void Enemy::SetPosition(int posX, int posY)
{
	/*b2Vec2 position = { PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY) };
	pbody->body->SetTransform(position, 0);*/
}