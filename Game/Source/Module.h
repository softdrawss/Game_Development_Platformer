#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"
#include <Optick/include/optick.h>

class App;
class PhysBody;
class GuiControl;

class Module
{
public:

	Module(bool startEnabled = false) : active(startEnabled)
	{}
	
	void Init()
	{
		//active = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		OPTICK_EVENT();
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		OPTICK_EVENT();
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		OPTICK_EVENT();
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	// L03: DONE 2: Create new virtual methods to LoadState / SaveState
	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	virtual void OnCollision(PhysBody* bodyA, PhysBody* bodyB)
	{

	}

	virtual void Enable()
	{
		if (!active)
		{
			active = true;
			Start();
		}
	}

	virtual void Disable()
	{
		if (active)
		{
			active = false;
			CleanUp();
		}
	}

	virtual bool OnGuiMouseClickEvent(GuiControl* control)
	{
		return true;
	}

public:

	SString name;
	bool active;

};

#endif // __MODULE_H__