#include "Coin.h"
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

Coin::Coin() : Entity(EntityType::COIN)
{
	name.Create("coin");
}

Coin::~Coin() {

}

bool Coin::Awake() {

	//L02: DONE 1: Initialize Player parameters
	//pos = position;
	//texturePath = "Assets/Textures/player/idle1.png";

	//L02: DONE 5: Get Player parameters from XML
	active = true;
	isPicked = false;
	return true;
}

bool Coin::Start()
{
	alive = true;
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();

	initPosition.x = position.x;
	initPosition.y = position.y;

	texturePath = parameters.attribute("texturepath").as_string();

	//initilize textures
	texture = app->tex->Load(texturePath);
	
	//Set bools

	// Pysics body
	pbody = app->physics->CreateCircleSensor(position.x, position.y, 7, bodyType::STATIC);
	pbody->listener = this;
	pbody->ctype = ColliderType::COIN;

	pbody->body->SetTransform(PIXEL_TO_METERS(initPosition), 0);
	alive = true;
	//initialize audio effect - !! Path is hardcoded, should be loaded from config.xml
	//pickCoinFxId = app->audio->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	LoadAnimations();

	return true;
}

bool Coin::Update(float dt)
{
	OPTICK_EVENT();
	b2Vec2 vel;
	int speed = 2;

	//Death
	if (!alive)
	{
		pbody->body->SetActive(false);
		pbody->body->SetType(b2_staticBody);
		//app->audio->PlayFx(deathaudio);
	}
	else {

		//Coin position
		position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 9;
		position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 18;

		SDL_Rect rect2 = currentAnim->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x+4, position.y+11, flip, &rect2);
		currentAnim->Update(dt);
	}

	return true;
}

bool Coin::CleanUp()
{
	app->entityManager->DestroyEntity(this);
	return true;
}

void Coin::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physB != nullptr) {
		if (physB->ctype == ColliderType::PLAYER) {
			if (!isPicked) {
				//app->audio->PlayFx(pickCoinAudio);
			}
			isPicked = true;

			LOG("COIN PICKED");
		}
	}
}

void Coin::SetPosition(int posX, int posY)
{
	b2Vec2 position = { PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY) };
	pbody->body->SetTransform(position, 0);
}

bool Coin::LookingRight()
{
	return app->scene->coin->flip == SDL_FLIP_NONE ? true : false;;
}

void Coin::LoadAnimations()
{
	rot.PushBack({ 0, 0, 16, 16 });
	rot.PushBack({ 16, 0, 16, 16 });
	rot.PushBack({ 32, 0, 16, 16 });
	rot.PushBack({ 48, 0, 16, 16 });
	rot.PushBack({ 64, 0, 16, 16 });
	rot.PushBack({ 80, 0, 16, 16 });
	rot.PushBack({ 96, 0, 16, 16 });
	rot.PushBack({ 112, 0, 16, 16 });

	rot.speed = 0.03f;


	currentAnim = &rot;
}


bool Coin::CheckPickingCoin()
{
	if (active && isPicked) {
		active = false;
		alive = false;
		return true;
	}
	return false;
}
