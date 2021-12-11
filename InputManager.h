#pragma once
#include <d3d11.h>
#include "Time.h"

class InputManager {
private:
	Time* _time;
	float _lastInput = 0;
	int _delay = 1;
	
public:
	InputManager();
	~InputManager();
	void SetTime(Time* time) { _time = time; };
	void Update();

	bool GetKeyDown(const char key);
	bool GetKeyUp(const char key);
};

