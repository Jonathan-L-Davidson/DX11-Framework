#pragma once
#include "Object.h"
#include "Window.h"
#include "Time.h"

class PlayerObject : public Object {
private:
	Window* _window;
	Time*	_time;

	bool _firstPersonView; // If the camera is in first person view mode
	bool _thirdPersonView; // If the camera is in third person view mode

	XMFLOAT3 _cameraOffsetFPS; // Offset for FirstPerson view
	XMFLOAT3 _cameraOffsetTPS; // Offset for ThirdPerson view

	XMFLOAT3 _cameraOffset; // buffer for the offset to allow things such as camera shake.

	Camera* _currentCamera;
	Camera* _staticCamera;
	Camera* _topDownCamera;
	Camera* _firstPersonCamera;
	Camera* _thirdPersonCamera;

	void CheckInput();
	void UpdateCameraOffset();
	void ChangeCamera(Camera* cam);
	void ChangeCameraPos(Camera* cam, XMVECTOR pos);
	void ChangeCameraRot(Camera* cam, XMVECTOR rot);

public:
	PlayerObject(XMFLOAT3 pos);
	~PlayerObject();

	void SetWindow(Window* window)	{ _window = window; };
	void SetTime(Time* time)		{ _time = time; };

	void Update(double dt);
};

