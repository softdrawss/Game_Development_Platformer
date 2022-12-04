#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Module.h"
#include "Point.h"
#include "Physics.h"

enum CameraMode
{
	PLAYER,
	CINEMATIC,
};

class Camera : public Module
{
public:

	Camera(bool startEnabled);
	virtual ~Camera();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	// Sets camera position at given x, y
	void SetPosition(int x, int y);
	// Moves camera a set amount x, y
	void AddPosition(int x, int y);

	void FollowPlayer();
	void CameraLimits();
	void FreeCamera();

public:

	int camSpeed;
	CameraMode cameraMode;

};

#endif // __CAMERA_H__