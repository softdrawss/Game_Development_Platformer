#include "App.h"

#include "Defs.h"
#include "Log.h"
#include <chrono>
using namespace std::chrono;
#include <thread>

// NOTE: SDL redefines main function
#include "SDL/include/SDL.h"
#include "Optick/include/optick.h"
#include "Debug.h"
// NOTE: Library linkage is configured in Linker Options
//#pragma comment(lib, "../Game/Source/External/SDL/libx86/SDL2.lib")
//#pragma comment(lib, "../Game/Source/External/SDL/libx86/SDL2main.lib")

#include <stdlib.h>

enum MainState
{
	CREATE = 1,
	AWAKE,
	START,
	LOOP,
	CLEAN,
	FAIL,
	EXIT
};

App* app = NULL;

int main(int argc, char* args[])
{
	LOG("Engine starting ...");
	
	MainState state = CREATE;
	int result = EXIT_FAILURE;

	while(state != EXIT)
	{
		high_resolution_clock::time_point startTime = high_resolution_clock::now();

		switch(state)
		{
			// Allocate the engine --------------------------------------------
			case CREATE:
			LOG("CREATION PHASE ===============================");

			app = new App(argc, args);

			if(app != NULL)
				state = AWAKE;
			else
				state = FAIL;

			break;

			// Awake all modules -----------------------------------------------
			case AWAKE:
			LOG("AWAKE PHASE ===============================");
			if(app->Awake() == true)
				state = START;
			else
			{
				LOG("ERROR: Awake failed");
				state = FAIL;
			}

			break;

			// Call all modules before first frame  ----------------------------
			case START:
			LOG("START PHASE ===============================");
			if(app->Start() == true)
			{
				state = LOOP;
				LOG("UPDATE PHASE ===============================");
			}
			else
			{
				state = FAIL;
				LOG("ERROR: Start failed");
			}
			break;

			// Loop all modules until we are asked to leave ---------------------
			case LOOP:
			//OPTICK_FRAME("Main Loop");
			if(app->Update() == false)
				state = CLEAN;
			break;

			// Cleanup allocated memory -----------------------------------------
			case CLEAN:
			LOG("CLEANUP PHASE ===============================");
			if(app->CleanUp() == true)
			{
				RELEASE(app);
				result = EXIT_SUCCESS;
				state = EXIT;
			}
			else
				state = FAIL;

			break;

			// Exit with errors and shame ---------------------------------------
			case FAIL:
			LOG("Exiting with errors :(");
			result = EXIT_FAILURE;
			state = EXIT;
			break;
		}
		
		if (state == LOOP && app->debug->controlFPS)
		{
			//FPS CONTROL
			high_resolution_clock::time_point endTime = high_resolution_clock::now();
			app->debug->timePerCycle = duration_cast<microseconds>(endTime - startTime);

			//Frames per microseconds of the desiredFPS
			app->debug->desiredFPSmic = (int)((1.0f / (float)app->debug->desiredFPS) * 1E6);

			//Sleep if the time is less than the desiredFPSmic
			if (app->debug->timePerCycle < std::chrono::microseconds(app->debug->desiredFPSmic))
			{
				std::this_thread::sleep_for(std::chrono::microseconds(std::chrono::microseconds(app->debug->desiredFPSmic) - app->debug->timePerCycle));
			}

			//Calculate the time with the delay
			endTime = high_resolution_clock::now();
			app->debug->elapsedFrame = duration_cast<microseconds>(endTime - startTime);
		}
		
	}
	
	LOG("... Bye! :)\n");
	
	// Dump memory leaks
	return result;
}