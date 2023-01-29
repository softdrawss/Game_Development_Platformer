#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"
#include "PerfTimer.h"
#include "Timer.h"

#include "PugiXml/src/pugixml.hpp"
#include "Optick/include/optick.h"

#define DEBUG_MODE false
#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Textures;
class Fonts;
class Audio;
class Physics;
class Particles;

class Logo;
class Title;
class Scene;
class Ending;
class GuiManager;
class Map;
class EntityManager;
class Camera;

class FadeToBlack;
class Debug;
class Render;

class PathFinding;
class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	pugi::xml_node GetNode();
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	// L03: DONE 1: Create methods to control that the real Load and Save happens at the end of the frame
	void LoadGameRequest();
	void SaveGameRequest() ;
	bool LoadFromFile();
	bool SaveToFile() ;
	bool CheckFileExists();
private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();


public:

	// Modules
	Window* win;
	Input* input;
	Textures* tex;
	Fonts* fonts;
	Audio* audio;
	Physics* physics;
	Particles* particles;

	Logo* logo;
	Title* titleScreen;
	Scene* scene;
	Ending* endScreen;
	GuiManager* guimanager;

	Map* map;
	EntityManager* entityManager;
	PathFinding* pathFinding;
	Camera* camera;

	FadeToBlack* fade;
	Debug* debug;
	Render* render;

	float secondsSinceStartup = 0.0f;
	
	float GetDeltaTime() { return dt; }

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module*> modules;

	// L01: DONE 2: Create new variables from pugui namespace:
	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	uint frames;
	bool vsync;
	float dt;

	// L03: DONE 1: Create control variables to control that the real Load and Save happens at the end of the frame
    bool saveGameRequested;
	bool loadGameRequested;

	// L13: DONE 4: Calculate some timing measures
	// required variables are provided:
	Timer timer;
	PerfTimer ptimer;

	Timer startupTime;
	Timer frameTime;
	Timer lastSecFrameTime;

	uint64 frameCount = 0;
	uint32 framesPerSecond = 0;
	uint32 lastSecFrameCount = 0;

	float averageFps = 0.0f;

	uint32 maxFrameDuration = 0;
};

extern App* app;

#endif	// __APP_H__