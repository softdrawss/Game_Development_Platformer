#include "NPCChickBoy.h"
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

NPCChickBoy::NPCChickBoy() : Entity(EntityType::NPC)
{
	name.Create("chickboy");
}

NPCChickBoy::~NPCChickBoy() {

}

bool NPCChickBoy::Awake() {

	//L02: DONE 5: Get Player parameters from XML

	return true;
}

bool NPCChickBoy::Start()
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
	pbody = app->physics->CreateCircle(position.x, position.y, 12, bodyType::DYNAMIC);

	// L07 DONE 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;
	pbody->ctype = ColliderType::NPC;

	LoadAnimations();

	return true;
}

bool NPCChickBoy::Update()
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
			currentAnim = &run;
			vel.x = -speed;
			isAsleep = false;
			flip = SDL_FLIP_HORIZONTAL;
		}
		//Right
		else if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
			currentAnim = &run;
			vel.x = speed;
			isAsleep = false;
			flip = SDL_FLIP_NONE;
		}
		else
			vel.x = 0;

	}

	//Set the velocity of the pbody of the enemy
	pbody->body->SetLinearVelocity(vel);

	//Set ChickBoy position
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 28;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 19;


	if (isAsleep) { currentAnim = &idle; }
	SDL_Rect rect2 = currentAnim->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y, flip, &rect2);
	currentAnim->Update();

	return true;
}

bool NPCChickBoy::PostUpdate()
{
	//For highscore

	return true;
}

bool NPCChickBoy::CleanUp()
{
	app->entityManager->DestroyEntity(this);
	return true;
}

void NPCChickBoy::OnCollision(PhysBody* physA, PhysBody* physB)
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

void NPCChickBoy::LoadAnimations()
{
	idle.PushBack({   0, 0, 32, 32 });
	idle.PushBack({  32, 0, 32, 32 });
	idle.PushBack({  64, 0, 32, 32 });
	idle.PushBack({  96, 0, 32, 32 });
	idle.PushBack({ 128, 0, 32, 32 });
	idle.PushBack({ 160, 0, 32, 32 });
	idle.speed = 0.1f;

	run.PushBack({   0, 32, 32, 32 });
	run.PushBack({  32, 32, 32, 32 });
	run.PushBack({  64, 32, 32, 32 });
	run.PushBack({  96, 32, 32, 32 });
	run.PushBack({ 128, 32, 32, 32 });
	run.PushBack({ 160, 32, 32, 32 });
	run.PushBack({ 196, 32, 32, 32 });
	run.PushBack({ 228, 32, 32, 32 });
	run.PushBack({ 260, 32, 32, 32 });
	run.PushBack({ 292, 32, 32, 32 });
	run.speed = 0.3f;

	currentAnim = &idle;
}

void NPCChickBoy::SetPosition(int posX, int posY)
{
	b2Vec2 position = { PIXEL_TO_METERS(posX), PIXEL_TO_METERS(posY) };
	pbody->body->SetTransform(position, 0);
}