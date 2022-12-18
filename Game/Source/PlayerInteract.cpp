#include "PlayerInteract.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"

PlayerInteract::PlayerInteract() : Entity(EntityType::INTERACT)
{
	name.Create("interact");
}

PlayerInteract::~PlayerInteract() {}

bool PlayerInteract::Awake()
{
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	//texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool PlayerInteract::Start()
{
	//initilize textures
	//texture = app->tex->Load(texturePath);
	
	// L07 DONE 4: Add a physics to an item - initialize the physics body
	pbody = app->physics->CreateRectangleSensor(position.x, position.y, 22, 16, bodyType::DYNAMIC);
	pbody->ctype = ColliderType::INTERACT;
	pbody->body->SetActive(false);
	pbody->listener = this;

	return true;
}

bool PlayerInteract::Update()
{
	// Set active?
	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		pbody->body->SetActive(true);
	else
		pbody->body->SetActive(false);

	//Set position
	b2Vec2 position;

	position.x = METERS_TO_PIXELS(app->scene->player->pbody->body->GetTransform().p.x) + 10;
	position.y = METERS_TO_PIXELS(app->scene->player->pbody->body->GetTransform().p.y);

	if (!app->scene->player->LookingRight())
		position.x -= 12;
	

	// L07 DONE 4: Add a physics to an item - update the position of the object from the physics. 
	pbody->body->SetTransform(PIXEL_TO_METERS(position), 0);
	
	
	//app->render->DrawTexture(texture, position.x, position.y, SDL_FLIP_NONE);

	return true;
}

bool PlayerInteract::CleanUp()
{
	return true;
}

void PlayerInteract::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::TRIG_1A:
		LOG("Collision TRIG_1A");
		
		break;
	case ColliderType::TRIG_2A:
		LOG("Collision TRIG_2A");
		break;
	case ColliderType::UNKNOWN:
		LOG("Collision UNKNOWN");
		break;
	}
}