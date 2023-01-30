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
#include "Particles.h"
#include "PlayerInteract.h"
#include "EntityManager.h"

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
	invulnerable = 2000;

	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	initPosition.x = position.x;
	initPosition.y = position.y;

	texturePath = parameters.attribute("texturepath").as_string();
	jumppath = parameters.attribute("jump").as_string();
	jumpaudio = app->audio->LoadFx(jumppath);
	
	
	deathpath = parameters.attribute("death").as_string();
	deathaudio = app->audio->LoadFx(deathpath);

	dashpath = parameters.attribute("dash").as_string();
	dashaudio = app->audio->LoadFx(dashpath);

	//initilize textures
	texture = app->tex->Load(texturePath);

	//Set bools
	remainingJumpSteps = 0;
	isIdle = true;
	remainingDash = 0;
	dashCD = 0;

	// Pysics body
	pbody = app->physics->CreateCircle(position.x, position.y, 7, bodyType::DYNAMIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	pbody->body->SetTransform(PIXEL_TO_METERS(initPosition), 0);
	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	//pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	LoadAnimations();

	return true;
}

bool Player::Update(float dt)
{
	OPTICK_EVENT();

	b2Vec2 vel;
	int speed = 2 * dt / 7;
	invulnerable -= dt;

	//God Mode
	if (app->debug->godMode)
	{
		alive = true;
		pbody->body->SetGravityScale(0);
	}
	else
	{
		pbody->body->SetGravityScale(1);

		if (vel.y > -GRAVITY_Y)
		{
			vel = b2Vec2(0, -GRAVITY_Y * dt / 16);
		}
		else
		{
			vel += b2Vec2(0, -GRAVITY_Y * dt / 20);
		}
	}

	//Death
	if (!alive)
	{
		pbody->body->SetActive(false);
		pbody->body->SetType(b2_staticBody);
		isIdle = false;
		currentAnim = &death;
		//app->audio->PlayFx(deathaudio);
	}
	else
	{
		isIdle = true;

		//Stairs
		if (stairs || app->debug->godMode)
		{
			//Up
			if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
				currentAnim = &idle;
				vel = b2Vec2(0, -speed);
				isIdle = false;
			}
			//Down
			else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
				currentAnim = &idle;
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
			remainingJumpSteps = 200;
			isIdle = false;
			isGrounded = false;			
			app->audio->PlayFx(jumpaudio);

		}
		//WallSlide - WallJump
		else if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !isGrounded)
		{
			//Wall Slide
			if (wallSlideRight && app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				vel.y += 4;
				//app->audio->PlayFx(slide);
				isIdle = true;
				isGrounded = false;
			}
			else if (wallSlideLeft && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				vel.y += 4;
				//app->audio->PlayFx(slide);
				isIdle = true;
				isGrounded = false;
			}

			//Wall Jump
			if (wallSlideRight && app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			{
				currentAnim = &jump;
				app->audio->PlayFx(jumpaudio);
				remainingJumpSteps = 200;
				isIdle = false;
				isGrounded = false;
			}
			else if (wallSlideLeft && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			{
				currentAnim = &jump;
				app->audio->PlayFx(jumpaudio);

				remainingJumpSteps = 200;
				isIdle = false;
				isGrounded = false;
			}		
		}

		//Dash
		if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN && dashCD == 0) {
			app->audio->PlayFx(dashaudio);
			remainingDash = 8;
		}
		else if (dashCD > 0){
			dashCD--;
		}

		//Attack
		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN && !isAttacking) {
			isAttacking = true;
			currentAnim = &attackNormal;
			//Attack();
			isIdle = false;
		}	
	}
	
	//Auto move right debug
	if (app->debug->moveRight)
		vel.x = speed;

	
	//Apply Jump Force
	if (remainingJumpSteps > 0)
	{
		vel.y = -0.000032 * remainingJumpSteps * remainingJumpSteps * dt;
		remainingJumpSteps -= dt;
	}
	else
		remainingJumpSteps = 0;

	//Dash end
	if (remainingDash > 0) {
		vel = b2Vec2(14 + remainingDash, 0);
		vel.x *= flip == SDL_FLIP_NONE ? 1 : -1;
		pbody->body->SetGravityScale(0);
		remainingDash--;

		if (remainingDash == 0)
		{
			vel = b2Vec2(0, 0);
			pbody->body->SetGravityScale(1);
			dashCD = 180;
		}
	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(b2Vec2(vel.x, vel.y));

	//Player position
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 9;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 18;

	//Animations
	if (isAttacking && attackNormal.GetCurrentFrameint() == 3)
	{
		attackNormal.Reset();
		isAttacking = false;
	}

	if (isIdle && !isAttacking) { currentAnim = &idle; }
	if (!isGrounded) { currentAnim = &jump; }
	if (isAttacking) { currentAnim = &attackNormal; }
	SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x+4, position.y+11, flip, &rect2);
	currentAnim->Update(dt);

	return true;
}

bool Player::PostUpdate()
{
	OPTICK_EVENT();
	
	return true;
}

bool Player::CleanUp()
{
	app->entityManager->DestroyEntity(this);
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
		if(invulnerable <= 0)
		{ 
			app->scene->healthCount -= 1;
			invulnerable = 2000;

			if (app->scene->healthCount == 0)
			{
				alive = false;
			}
		}			
		break;
	case ColliderType::GROUND:
		LOG("Collision GROUND");
		isGrounded = true;
		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		//app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		isGrounded = true;
		break;
	case ColliderType::WALL:
		LOG("Collision WALL");
		//player has a wall on LEFT?
		if (physA->body->GetTransform().p.x > physB->body->GetTransform().p.x)
			wallSlideLeft = true;
		else
			wallSlideRight = true;
		break;
	case ColliderType::COIN:
		LOG("Collision COIN");
		app->audio->PlayFx(app->scene->pickCoinAudio_sfx);
		break;

		//TRIGGER
	case ColliderType::TRIG_1A:
		LOG("Save checkpoint 1");
		app->SaveGameRequest();
		break;

	case ColliderType::TRIG_2A:
		LOG("Save checkpoint 2");
		app->SaveGameRequest();
		break;

	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
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
	//pbody->body->SetActive(true);
}

bool Player::LookingRight()
{
	return app->scene->player->flip == SDL_FLIP_NONE ? true : false;;
}

void Player::LoadAnimations()
{
	idle.PushBack({ 0, 0, 15, 16 });
	idle.PushBack({ 16, 0, 15, 16 });
	idle.PushBack({ 32, 0, 15, 16 });
	idle.speed = 0.03f;


	// Run animation, there is also a walk animation
	// but as we are not using anything that changes the speed I'm going to stick to walk
	run.PushBack({ 0, 16, 15, 16 });
	run.PushBack({ 16, 16, 15, 16 });
	run.PushBack({ 32, 16, 15, 16 });
	run.PushBack({ 48, 16, 15, 16 });
	run.PushBack({ 64, 16, 15, 16 });
	run.PushBack({ 80, 16, 15, 16 });
	run.PushBack({ 96, 16, 15, 16 });
	run.PushBack({ 112, 16, 15, 16 });
	run.speed = 0.15f;

	//climb.PushBack({ 0, 128, 32, 32 });
	//climb.PushBack({ 32, 128, 32, 32 });
	//climb.PushBack({ 64, 128, 32, 32 });
	//climb.PushBack({ 96, 128, 32, 32 });
	//climb.speed = 0.1f;
	
	jump.PushBack({ 48, 0, 15, 16 });
	jump.PushBack({ 64, 0, 15, 16 });
	jump.PushBack({ 80, 0, 15, 16 });
	jump.PushBack({ 96, 0, 15, 16 });
	jump.PushBack({ 112, 0, 15, 16 });
	jump.speed = 0.1f;
	jump.loop;
	
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
	
	attackNormal.PushBack({ 0, 48, 15, 16 });
	attackNormal.PushBack({ 16, 48, 15, 16 });
	attackNormal.PushBack({ 32, 48, 15, 16 });
	attackNormal.PushBack({ 48, 48, 15, 16 });
	
	attackNormal.speed = 0.1f;
	attackNormal.loop = false;

	currentAnim = &idle;
}