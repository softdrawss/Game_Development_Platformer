#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

enum class GuiButtontype {
	PLAY,
	CONTINUE,
	SETTINGS,
	CREDITS,
	EXIT,
	RESUME,
	BACK_TO_TITLE
};

class GuiButton : public GuiControl
{
public:

	GuiButton(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiButton();

	bool Update(float dt);
	bool Draw(Render* render);

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	uint pressed, focused;

public:
	GuiButtontype button;
};

#endif // __GUIBUTTON_H__