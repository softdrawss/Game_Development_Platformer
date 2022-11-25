#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "Module.h"

#define DEBUG_BOX 225

class Debug : public Module
{
public:

	Debug(bool startEnabled);
	virtual ~Debug();

	bool Start();
	bool Update(float dt);	// Switch debug on/off
	bool PostUpdate();		// Draw if debug true

	void DrawDebug();
	void DrawColliders();

public:
	//flags
	bool debug;
	bool variables = false;
	bool camLimits = false;
	bool godMode = false;
	bool freeCam = false;
	bool teleport = false;
	bool drawColliders = false;

private:

	
};

#endif // __DEBUG_H__