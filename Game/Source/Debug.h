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

public:
	//flags
	bool debug;
	bool godMode = false;
	bool debugCam = false;
	bool variables = false;
	bool camLimits = false;
	bool teleport = false;

private:

	
};

#endif // __DEBUG_H__