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
#include "Map.h"
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
	audiopath = parameters.attribute("audioPath").as_string();
	audio = app->audio->LoadFx(audiopath);

	//initilize textures
	texture = app->tex->Load(texturePath);

	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircleSensor(position.x, position.y, 8, bodyType::DYNAMIC);


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

	int speed = 1;
	vel.y = 0;
	//vel = pbody->body->GetLinearVelocity() + b2Vec2(0, -GRAVITY_Y * 0.0166);
	
	if (!alive)
	{
		isIdle = false;
		currentAnim = &death;
		app->entityManager->DestroyEntity(this);
	}
	else
	{
		isIdle = true;

		// PATHFINDING
		iPoint enemyTile = app->map->WorldToMap(
			METERS_TO_PIXELS(this->pbody->body->GetPosition().x),
			METERS_TO_PIXELS(this->pbody->body->GetPosition().y));

		iPoint playerTile = app->map->WorldToMap(
			METERS_TO_PIXELS(app->scene->player->pbody->body->GetPosition().x),
			METERS_TO_PIXELS(app->scene->player->pbody->body->GetPosition().y));

		if (lastEnemyTile != enemyTile || *(path.At(path.Count() - 1)) != playerTile)
		{
			path.Clear();
			app->pathFinding->CreatePath(enemyTile, playerTile);

			//Save path
			const DynArray<iPoint>* lastPath = app->pathFinding->GetLastPath();
			for (uint i = 0; i < lastPath->Count(); i++)
			{
				path.PushBack(iPoint(lastPath->At(i)->x, lastPath->At(i)->y));
			}
		}
		
		if (app->debug->drawColliders)
		{
			//Draw path
			for (uint i = 0; i < path.Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path.At(i)->x, path.At(i)->y);
				SDL_Rect pathTile{ pos.x, pos.y, 16, 16 };
				app->render->DrawRectangle(pathTile, 255, 255, 255, 64);
			}
		}
		

		// MOVEMENT
		
		if (path.Count() > 1)
		{
			iPoint dir;
			dir.Create(path.At(1)->x - path.At(0)->x, path.At(1)->y - path.At(0)->y);
			

			//Left
			if (dir.x < 0) {
				vel.x = -speed;
				flip = SDL_FLIP_HORIZONTAL;
			}
			//Right
			else if (dir.x > 0) {
				vel.x = speed;
				flip = SDL_FLIP_NONE;
			}
			

			//Up
			if (dir.y < 0) {
				vel.y = -speed;
				flip = SDL_FLIP_HORIZONTAL;
			}
			//Down
			else if (dir.y > 0) {
				vel.y = speed;
				flip = SDL_FLIP_NONE;
			}
		}
		
		
	}

	//Set the velocity of the pbody of the enemy
	pbody->body->SetGravityScale(0);
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 31;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 22;

	//Animation
	SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x+20, position.y+16, flip, &rect2);
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
	case ColliderType::INTERACT:
		LOG("Collision INTERACT");
		alive = false;
		app->audio->PlayFx(audio);

		break;
	case ColliderType::ITEM:
		LOG("Collision ITEM");
		//app->audio->PlayFx(pickCoinFxId);
		break;
	case ColliderType::PLATFORM:
		LOG("Collision PLATFORM");
		//isGrounded = true;
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

void EnemyFly::LoadAnimations()
{
	idle.PushBack({  0, 80, 16, 16 });
	idle.PushBack({ 16, 80, 16, 16 });
	idle.PushBack({ 32, 80, 16, 16 });
	idle.PushBack({ 48, 80, 16, 16 });
	idle.speed = 0.08f;

	currentAnim = &idle;
}

void EnemyFly::SetPosition(int posX, int posY)
{
	b2Vec2 position = { PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY) };
	pbody->body->SetTransform(position, 0);
}

bool EnemyFly::PlayerInRange(double range)
{
	double distance = sqrt(pow(app->scene->player->position.x - position.x, 2) + pow(app->scene->player->position.y - position.y, 2) * 1.0);

	return distance < range ? true : false;
}