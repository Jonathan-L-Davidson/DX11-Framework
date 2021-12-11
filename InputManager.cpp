#include "InputManager.h"
#include <ios>

InputManager::InputManager() {
}

InputManager::~InputManager() {
}

bool InputManager::GetKeyDown(const char key) {
	if (!GetFocus()) return false;
	if (_lastInput <= 0) {
		if (GetAsyncKeyState(key) & WM_SYSKEYDOWN) {
			_lastInput = _delay;
			return true;
		}
	}
	return false;
}

bool InputManager::GetKeyUp(const char key) {
	if (!GetFocus()) return false;
	if (GetAsyncKeyState(key) & WM_SYSKEYUP) { 
		return true;
	}

	return false;
}

void InputManager::Update() {
	if (_lastInput > 0) {
		_lastInput -= _time->GetDelta();
	}
}