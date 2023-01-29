#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"

enum class GuiSlidertype {
	FX,
	MUSIC
};


class GuiSlider : public GuiControl
{
public:

	GuiSlider(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiSlider();

	bool Update(float dt);
	bool Draw(Render* render);
	void SetValue(int value);

	int mouseX, mouseY, previousX, previousY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	int length, position;

	uint audioFxId;
	SDL_Rect square;

	GuiSlidertype slider;
	SDL_Rect sliderLine;

public:
};

#endif // __GUISLIDER_H__