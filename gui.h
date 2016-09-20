#ifndef _GUI_H
#define _GUI_H

#include <directx9GUIRenderer/d3d9renderer.h>
#include "character_creation.h"

class GUI {
public:
	GUI(LPDIRECT3DDEVICE9 d3Device, CharacterCreation* character_creation);
	~GUI();
	void renderGUI();
	void injectTimePulse(float time_elapsed);
	void injectMouseMove(int x, int y);
	void injectMouseButton(int retButton1, int retButton2, int retButton3, int oldRetButton1, int oldRetButton2, int oldRetButton3);
	void injectChar(int key_code);
	bool show();
	const bool isCharacterMenuShown() const;
	bool handleSubmit(const CEGUI::EventArgs& event);
	void updateCharacteristicSheet();
private:
	LPDIRECT3DDEVICE9 d3Device;
	CharacterCreation* character_creation;
};

#endif