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
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	return true;
}

bool Player::Start() {

	//initilize textures
	texture = app->tex->Load(texturePath);
	//id = app->tex->LoadSprite(texturePath, 15, 8);
	
	// L07 DONE 5: Add physics to the player - initialize physics body
	pbody = app->physics->CreateCircle(position.x+16, position.y+16, 16, bodyType::DYNAMIC);

	// Animations: Still do not work, something related to how the texture is shown
	left.PushBack({ 0, 160, 32, 32 });
	left.PushBack({ 32, 160, 32, 32 });
	left.PushBack({ 64, 160, 32, 32 });
	left.PushBack({ 96, 160, 32, 32 });
	left.speed = 0.1f;

	//right

	// Run animation, there is also a walk animation but as we are not using anything that changes the speed I'm going to stick to walk
	LRun.PushBack({ 0, 64, 32, 32 });
	LRun.PushBack({ 32, 64, 32, 32 });
	LRun.PushBack({ 64, 64, 32, 32 });
	LRun.PushBack({ 96, 64, 32, 32 });
	LRun.PushBack({ 128, 64, 32, 32 });
	LRun.PushBack({ 160, 64, 32, 32 });
	LRun.speed = 0.1f;

	//RRun

	climb.PushBack({ 0, 128, 32, 32 });
	climb.PushBack({ 32, 128, 32, 32 });
	climb.PushBack({ 64, 128, 32, 32 });
	climb.PushBack({ 96, 128, 32, 32 });
	climb.speed = 0.1f;

	LJump.PushBack({ 224, 32, 32, 32 });
	LJump.PushBack({ 256, 32, 32, 32 });
	LJump.PushBack({ 288, 32, 32, 32 });
	LJump.PushBack({ 320, 32, 32, 32 });
	LJump.PushBack({ 352, 32, 32, 32 });
	LJump.PushBack({ 384, 32, 32, 32 });
	LJump.PushBack({ 416, 32, 32, 32 });
	LJump.PushBack({ 448, 32, 32, 32 });
	LJump.speed = 0.1f;

	//RJump

	death.PushBack({ 224, 128, 32, 32 });
	death.PushBack({ 256, 128, 32, 32 });
	death.PushBack({ 288, 128, 32, 32 });
	death.PushBack({ 320, 128, 32, 32 });
	death.PushBack({ 352, 128, 32, 32 });
	death.PushBack({ 384, 128, 32, 32 });
	death.PushBack({ 416, 128, 32, 32 });
	death.PushBack({ 448, 128, 32, 32 });
	death.loop = false;
	death.speed = 0.1f;

	currentAnim = &left;

	return true;
}

bool Player::Update()
{

	// L07 DONE 5: Add physics to the player - updated player position using physics
	SDL_Rect rect2 = currentAnim->GetCurrentFrame();

	int speed = 5; 
	b2Vec2 vel = b2Vec2(0, -GRAVITY_Y); 

	// L02: DONE 4: modify the position of the player using arrow keys and render the texture
	// Maybe can be used to go up through some stairs (?)
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		vel = b2Vec2(0, -speed);
	}
	// Honestly this won't be used
	// Could be used to crouch (maybe to go through passages where the player cannot stand up?)
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		vel = b2Vec2(0, speed);
	}
		
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) {
		vel = b2Vec2(-speed, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) {
		vel = b2Vec2(speed, -GRAVITY_Y);
	}

	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT) {

	}
	
	// F1/F2: Start from the first/second level
	if (app->input->GetKey(SDL_SCANCODE_F1) == KEY_REPEAT) {

	}
	
	if (app->input->GetKey(SDL_SCANCODE_F2) == KEY_REPEAT) {

	}
	
	// F3: Start from the beginning of the current level
	if (app->input->GetKey(SDL_SCANCODE_F3) == KEY_REPEAT) {

	}

	// F5: Save the current game state
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_REPEAT) {
		savedPosition.x = position.x;
		savedPosition.y = position.y;
	}

	// F6: Load the previous state (even across levels)
	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_REPEAT) {

	}

	// F9: View colliders / logic
	if (app->input->GetKey(SDL_SCANCODE_F9) == KEY_REPEAT) {

	}

	// F10: God Mode (fly around, cannot be killed, etc)
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_REPEAT) {

	}

	//Set the velocity of the pbody of the player
	pbody->body->SetLinearVelocity(vel);

	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;

	app->render->Blit(texture, position.x, position.y, &rect2);

	return true;
}

bool Player::CleanUp()
{

	return true;
}