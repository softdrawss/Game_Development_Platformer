#ifndef __INPBUTTON_H__
#define __INPBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Render.h"
#include <string>

class InpButton : public GuiControl
{
public:

	InpButton(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~InpButton();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;
	bool renderText = false;

	std::string input;
	uint audioFxId;

	//Scene textures
	SDL_Texture* gPromptTextTexture;
	SDL_Texture* gInputTextTexture;
};

#endif // __GUIBUTTON_H__