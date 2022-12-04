#include "Player.h"
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

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML

	return true;
}

bool Player::Start()
{
	alive = true;
	stairs = false;
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	initPosition.x = position.x;
	initPosition.y = position.y;

	texturePath = parameters.attribute("texturepath").as_string();
	
	//initilize textures
	texture = app->tex->Load(texturePath);

	remainingJumpSteps = 0;
	isIdle = true;
	
	// Pysics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 12, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pbody->body->SetTransform(PIXEL_TO_METERS(initPosition), 0);
	alive = true;
	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	//pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	LoadAnimations();

	return true;
}

bool Player::Update()
{
	b2Vec2 vel;
	int speed = 5;

	//God Mode
	if (app->debug->godMode)
	{
		alive = true;
		pbody->body->SetGravityScale(0);
	}
	else
	{
		pbody->body->SetGravityScale(1);
		vel = pbody->body->GetLinearVelocity() + b2Vec2(0, -GRAVITY_Y * 0.0166);
	}

	//Death
	if (!alive)
	{
		pbody->body->SetActive(false);
		pbody->body->SetType(b2_staticBody);
		isIdle = false;
		currentAnim = &death;
	}
	else
	{
		isIdle = true;

		//Stairs
		if (stairs || app->debug->godMode)
		{
			//Up
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				currentAnim = &climb;
				vel = b2Vec2(0, -speed);
				isIdle = false;
			}
			//Down
			else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				currentAnim = &climb;
				vel = b2Vec2(0, speed);
				isIdle = false;
			}
			else
				vel.y = 0;
		}

		//Left
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
			currentAnim = &run;
			vel.x = -speed;
			isIdle = false;
			flip = SDL_FLIP_HORIZONTAL;
		}
		//Right
		else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
			currentAnim = &run;
			vel.x = speed;
			isIdle = false;
			flip = SDL_FLIP_NONE;
		}
		else
			vel.x = 0;

		//Jump
		if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && isGrounded && remainingJumpSteps == 0) {
			currentAnim = &jump;
			remainingJumpSteps = 6;
			isIdle = false;
			isGrounded = false;
		}

		//Attack
		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && !isAttackingRock) {
			Attack();
			isAttackingRock = true;
			currentAnim = &attackRock;
			isIdle = false;
		}	
	}
	
	//Auto move right debug
	if (app->debug->moveRight)
		vel.x = speed;

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Apply Jump Force
	if (remainingJumpSteps > 0)
	{
		float force = pbody->body->GetMass() * 10 / 0.01666; //F = mv/t (t = 1/60fps)
		force /= 6.0;
		pbody->body->ApplyForce(b2Vec2(0, -force), pbody->body->GetWorldCenter(), true);
		remainingJumpSteps--;	
	}

	//Player position
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 18;

	//Animations
	if (isAttackingRock && attackRock.GetCurrentFrameint() == 4)
	{
		attackRock.Reset();
		isAttackingRock = false;
	}

	if (isIdle && !isAttackingRock) { currentAnim = &idle; }
	if (!isGrounded) { currentAnim = &jump; }
	if (isAttackingRock) { currentAnim = &attackRock; }
	SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, flip, &rect2);
	currentAnim->Update();

	return true;
}

bool Player::PostUpdate()
{
	
	
	return true;
}

bool Player::CleanUp()
{
	app->tex->UnLoad(texture);
	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::DEATH:
		LOG("Collision DEATH");
		alive = false;
		break;
	case ColliderType::ENEMY:
		//need to tell it, depending on how we kill (or make disappear) enemies
		//will we shoot to them? or more like mario, smashing them up to the ground with our weight?
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
	case ColliderType::WALL:
		LOG("Collision WALL");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}

void Player::LoadAnimations()
{
	idle.PushBack({ 0, 160, 32, 32 });
	idle.PushBack({ 32, 160, 32, 32 });
	idle.PushBack({ 64, 160, 32, 32 });
	idle.PushBack({ 96, 160, 32, 32 });
	idle.speed = 0.08f;

	// Run animation, there is also a walk animation
	// but as we are not using anything that changes the speed I'm going to stick to walk
	run.PushBack({ 0, 64, 32, 32 });
	run.PushBack({ 32, 64, 32, 32 });
	run.PushBack({ 64, 64, 32, 32 });
	run.PushBack({ 96, 64, 32, 32 });
	run.PushBack({ 128, 64, 32, 32 });
	run.PushBack({ 160, 64, 32, 32 });
	run.speed = 0.1f;

	climb.PushBack({ 0, 128, 32, 32 });
	climb.PushBack({ 32, 128, 32, 32 });
	climb.PushBack({ 64, 128, 32, 32 });
	climb.PushBack({ 96, 128, 32, 32 });
	climb.speed = 0.1f;

	jump.PushBack({ 224, 32, 32, 32 });
	jump.PushBack({ 256, 32, 32, 32 });
	jump.PushBack({ 288, 32, 32, 32 });
	jump.PushBack({ 320, 32, 32, 32 });
	jump.PushBack({ 352, 32, 32, 32 });
	jump.PushBack({ 384, 32, 32, 32 });
	jump.PushBack({ 416, 32, 32, 32 });
	jump.PushBack({ 448, 32, 32, 32 });
	jump.speed = 0.2f;
	jump.loop;

	death.PushBack({ 224, 128, 32, 32 });
	death.PushBack({ 256, 128, 32, 32 });
	death.PushBack({ 288, 128, 32, 32 });
	death.PushBack({ 320, 128, 32, 32 });
	death.PushBack({ 352, 128, 32, 32 });
	death.PushBack({ 384, 128, 32, 32 });
	death.PushBack({ 416, 128, 32, 32 });
	death.PushBack({ 448, 128, 32, 32 });
	death.PushBack({ 480, 128, 32, 32 });
	death.speed = 0.1f;
	death.loop = false;

	attackRock.PushBack({ 224, 192, 32, 32 });
	attackRock.PushBack({ 256, 192, 32, 32 });
	attackRock.PushBack({ 288, 192, 32, 32 });
	attackRock.PushBack({ 320, 192, 32, 32 });
	attackRock.PushBack({ 352, 192, 32, 32 });
	attackRock.speed = 0.3f;
	attackRock.loop = false;

	currentAnim = &idle;
}

void Player::SetPosition(int posX, int posY)
{
	b2Vec2 position = { PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY) };
	pbody->body->SetTransform(position, 0);
}

void Player::Attack() {

	//IDEAS
	//shoot them with the animation of shooting (but we have to calibrate where stones are going)
	//Specially if the player has to shoot to a flying enemy, and do the animations for that
	//Jump onto them if not
}