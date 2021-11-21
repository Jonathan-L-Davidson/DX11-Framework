#pragma once
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include <stdio.h>
#include <vector>
#include "resource.h"
#include "TextureLoader.h"
#include "OBJLoader.h"
#include "Structures.h"
#include "TextureManager.h"

class Shader {
	ID3D11VertexShader* _pVertexShader;
	ID3D11PixelShader* _pPixelShader;
	ID3D11InputLayout* _pVertexLayout;
	ID3D11SamplerState* _pSamplerLinear;
};

class Object {
private:
	XMFLOAT4X4	_position;
	XMFLOAT4X4	_rotation;
	XMFLOAT4X4	_scale;
	MeshData*	_meshData;
	Shader*		_shader;
	Texture*	_texture;

	XMFLOAT4X4	_world;
public:
	UINT id;

public:
	Object();
	~Object();

	void Destroy();
	void LoadModel(MeshData* mesh);
	void LoadShader(Shader* shader);

	void Update();
	void Draw(ID3D11DeviceContext* immediateContext, XMMATRIX view, XMMATRIX projection, float t);

	void UpdateWorldCoords();

public:
	///
	// Position helpers!
	///
	void AddPos(XMVECTOR pos) { 
		XMMATRIX bufferPos = GetPos(); // Make an old copy of the current position.
		XMStoreFloat4x4(&_position, XMMatrixTranslationFromVector(pos) * bufferPos); // Multiply the old matrice ontop of the old one..?
		// TO DO, check if this works!
	};
	
	void SetPos(XMVECTOR pos) { XMStoreFloat4x4(&_position, XMMatrixTranslationFromVector(pos)); };
	XMMATRIX GetPos() { return XMLoadFloat4x4(&_position); };

	///
	// Rotation Helpers!
	///
	void AddRotation(XMVECTOR rotation) { 
		XMMATRIX bufferRot = GetRotation();
		XMStoreFloat4x4(&_rotation, XMMatrixRotationRollPitchYawFromVector(rotation) * bufferRot);
	};

	void SetRotation(XMVECTOR rotation) { XMStoreFloat4x4(&_rotation, XMMatrixRotationRollPitchYawFromVector(rotation)); };
	XMMATRIX GetRotation() { return XMLoadFloat4x4(&_rotation); };

	///
	// Scale Helpers!
	///
	void AddScale(XMVECTOR scale) {
		XMMATRIX bufferScale = GetScale();
		XMStoreFloat4x4(&_scale, XMMatrixScalingFromVector(scale) * bufferScale);
	};
	
	void SetScale(XMVECTOR scale) { XMStoreFloat4x4(&_scale, XMMatrixScalingFromVector(scale)); };
	XMMATRIX GetScale() { return XMLoadFloat4x4(&_scale); };

	XMMATRIX GetWorld() { return XMLoadFloat4x4(&_world); };
};

class ObjectManager {
private:
	std::vector<Object*>* objects;

public:
	ObjectManager();
	~ObjectManager();

	AddObject();
};

