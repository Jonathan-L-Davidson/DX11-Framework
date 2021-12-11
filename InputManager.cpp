#include "InputManager.h"

InputManager::InputManager() {
}

InputManager::~InputManager() {
}

bool InputManager::GetKeyDown(int key) {
	if (!GetFocus()) return false;
	if (GetKeyState(key) & WM_KEYDOWN && _lastInput <= 0) {
		_lastInput = _delay;
		return true;
	}
	return false;
}

bool InputManager::GetKeyUp(int key) {
	if (!GetFocus()) return false;
	if (GetKeyState(key) & WM_KEYUP && _lastInput <= 0) {
		_lastInput = _delay;
		return true;
	}
	return false;
}

void InputManager::Update() {
	if (_lastInput > 0) _lastInput--;
}