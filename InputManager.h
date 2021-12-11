#pragma once
#include <winuser.h>
#include <d3d11.h>
#include "Window.h"

class InputManager {
private:
	int _lastInput = 0;
	int _delay = 100;
	
public:
	InputManager();
	~InputManager();
	void Update();

	bool GetKeyDown(int key);
	bool GetKeyUp(int key);
};

