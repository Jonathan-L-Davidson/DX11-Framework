#include "Camera.h"

Camera::Camera() {
	Eye = XMVectorSet(0.0f, 5.0f, -30.0f, 0.0f);
	At = XMVectorSet(0.0f, -3.0f, 0.0f, 0.0f);
	Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
}

Camera::~Camera() {
	return;
}