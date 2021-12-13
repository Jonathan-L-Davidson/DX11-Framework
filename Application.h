#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include "OBJLoader.h"
#include "Structures.h"
#include "GraphicManager.h"
#include "TextureManager.h"
#include "ObjectManager.h"
#include "InputManager.h"
#include "SpinningObject.h"
#include "HardcodedObjects.h"
#include "Time.h"

using namespace DirectX;


class Application {
private:
	HINSTANCE               _hInst;
	GraphicManager*			_graphicManager;
	TextureManager*			_textureManager;
	ObjectManager*			_objectManager;
	InputManager*			_inputManager;
	Time*					_time;

	SpinningObject*			_spinningObject;
	Pyramid*				_pyramid;
	Cube*					_cube;

	HRESULT SetupManagers(HINSTANCE hInstance, int nCmdShow);
	void SetupObjects();

private:
	void Cleanup();
public:
	Application();
	~Application();

	HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

	void Update();
	void Draw();
};

