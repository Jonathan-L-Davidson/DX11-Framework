#pragma once
#include "Object.h"
#include "Window.h"
#include "InputManager.h"
#include "Time.h"

using namespace DirectX;

class PlayerObject : public Object {
private:
	InputManager* _input;
	Window* _window;

	float _moveSpeed = 100;
	float _turnSpeed = 5;

	bool _firstPersonView = false; // If the camera is in first person view mode
	bool _thirdPersonView = false; // If the camera is in third person view mode

	XMFLOAT3 _cameraOffsetFPS; // Offset for FirstPerson view
	XMFLOAT3 _cameraOffsetTPS; // Offset for ThirdPerson view

	XMFLOAT3 _cameraOffset; // buffer for the offset to allow things such as camera shake.
	XMVECTOR _cameraPosition;

	Camera* _currentCamera;
	Camera* _staticCamera;
	Camera* _topDownCamera;
	Camera* _firstPersonCamera;
	Camera* _thirdPersonCamera;

	void CheckInput(double dt);
	XMFLOAT3 HandleMovementInput(double dt);
	XMFLOAT3 HandleViewInput(double dt);
	void UpdateCameraOffset();
	void ChangeCamera(Camera* cam);

public:
	PlayerObject(XMFLOAT3 pos);
	~PlayerObject();

	void Initialise();
	void SetInput(InputManager* input) { _input = input; };
	void SetWindow(Window* window)	{ _window = window; };

	void Update(double dt);
};

