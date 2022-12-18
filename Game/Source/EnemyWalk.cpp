#include "EnemyWalk.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "PathFinding.h"
#include "Map.h"
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
	audiopath = parameters.attribute("audioPath").as_string();
	audio = app->audio->LoadFx(audiopath);

	isAsleep = true;

	// L07 DONE 5: Add physics to the enemy - initialize physics body
	pbody = app->physics->CreateRectangle(position.x, position.y, 13, 13, bodyType::DYNAMIC);

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
		vel = pbody->body->GetLinearVelocity() + b2Vec2(0, -GRAVITY_Y * 0.0166);


		if (PlayerInRange(160))
		{
			// PATHFINDING
			iPoint enemyTile = app->map->WorldToMap(
				METERS_TO_PIXELS(this->pbody->body->GetPosition().x),
				METERS_TO_PIXELS(this->pbody->body->GetPosition().y));

			iPoint playerTile = app->map->WorldToMap(
				METERS_TO_PIXELS(app->scene->player->pbody->body->GetPosition().x),
				METERS_TO_PIXELS(app->scene->player->pbody->body->GetPosition().y));

			app->pathFinding->CreatePath(enemyTile, playerTile);
			path.Clear();

			//Save path
			const DynArray<iPoint>* lastPath = app->pathFinding->GetLastPath();
			for (uint i = 0; i < lastPath->Count(); i++)
			{
				path.PushBack(iPoint(lastPath->At(i)->x, lastPath->At(i)->y));
			}

			//Draw path
			for (uint i = 0; i < path.Count(); ++i)
			{
				iPoint pos = app->map->MapToWorld(path.At(i)->x, path.At(i)->y);
				SDL_Rect pathTile{ pos.x, pos.y, 16, 16 };
				app->render->DrawRectangle(pathTile, 255, 255, 255, 64);
			}
		}
		
		// MOVEMENT
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

		////Start charge
		//if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT && isCharging == 0 && !isShooting)
		//{
		//	isCharging = 120;
		//	currentAnim = &shoot;
		//	isAsleep = false;
		//}

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
	//if (isShooting && shoot.GetCurrentFrameint() == 3)
	//{
	//	shoot.Reset();
	//	isShooting = false;
	//}

	if (isAsleep) { currentAnim = &idle; }
	//if (isCharging != 0) { currentAnim = &charge; }
	//if (isShooting) { currentAnim = &shoot; }
	SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x+20, position.y+11, flip, &rect2);
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
	case ColliderType::INTERACT:
		LOG("Collision INTERACT");
		alive = false;
		app->audio->PlayFx(audio);
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
		//alive = false;
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

	idle.PushBack({  0, 48, 16, 16 });
	idle.PushBack({ 16, 48, 16, 16 });
	idle.PushBack({ 32, 48, 16, 16 });
	idle.PushBack({ 48, 48, 16, 16 });
	idle.PushBack({ 64, 48, 16, 16 });
	idle.speed = 0.1f;

	move.PushBack({ 112, 48, 16, 16 });
	move.PushBack({ 128, 48, 16, 16 });
	move.PushBack({ 144, 48, 16, 16 });
	move.PushBack({ 160, 48, 16, 16 });
	move.PushBack({ 176, 48, 16, 16 });
	move.PushBack({ 192, 48, 16, 16 });
	move.speed = 0.2f;


	//hit.PushBack({  0, 150, 55, 30 });
	//hit.PushBack({ 55, 150, 55, 30 });
	//hit.loop = false;
	//hit.speed = 0.2f;

	//death.PushBack({   0, 180, 55, 30 });
	//death.PushBack({  55, 180, 55, 30 });
	//death.PushBack({ 110, 180, 55, 30 });
	//death.PushBack({ 165, 180, 55, 30 });
	//death.PushBack({ 220, 180, 55, 30 });
	//death.PushBack({ 275, 180, 55, 30 });
	//death.loop = false;
	//death.speed = 0.2f;

	currentAnim = &idle;
}

void EnemyWalk::SetPosition(int posX, int posY)
{
	b2Vec2 position = { PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY) };
	pbody->body->SetTransform(position, 0);
	pbody->body->SetType(b2_dynamicBody);

}

bool EnemyWalk::PlayerInRange(double range)
{
	double distance = sqrt(pow(app->scene->player->position.x - position.x, 2) + pow(app->scene->player->position.y - position.y, 2) * 1.0);

	return distance < range ? true : false;
}
