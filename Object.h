#pragma once
#include <DirectXMath.h>
#include "OBJLoader.h"
#include "TextureManager.h"


class Object {
private:
	UINT _id;

	XMFLOAT4X4	_position;
	XMFLOAT4X4	_rotation;
	XMFLOAT4X4	_scale;
	MeshData* _meshData;
	Shader* _shader;
	Texture* _texture;

	XMFLOAT4X4	_world;
public:

public:
	Object();
	~Object();

	void Destroy();
	void LoadModel(MeshData* mesh);
	void LoadTexture(Texture* texture);
	void LoadShader(Shader* shader);

	void Update();
	void Draw(ID3D11DeviceContext* immediateContext, ID3D11Buffer* constantBuffer, XMFLOAT4X4 view, XMFLOAT4X4 projection, float t);

	void UpdateWorldCoords();

public:
	///
	// Position helpers!
	///
	void AddPos(XMVECTOR pos) {
		XMMATRIX bufferPos = XMLoadFloat4x4(&GetPos()); // Make an old copy of the current position.
		XMStoreFloat4x4(&_position, XMMatrixTranslationFromVector(pos) * bufferPos); // Multiply the old matrice ontop of the old one..?
		// TO DO, check if this works!
	};

	void SetPos(XMVECTOR pos) { XMStoreFloat4x4(&_position, XMMatrixTranslationFromVector(pos)); };
	XMFLOAT4X4 GetPos() { return _position; };

	///
	// Rotation Helpers!
	///
	void AddRotation(XMVECTOR rotation) {
		XMMATRIX bufferRot = XMLoadFloat4x4(&GetRotation());
		XMStoreFloat4x4(&_rotation, XMMatrixRotationRollPitchYawFromVector(rotation) * bufferRot);
	};

	void SetRotation(XMVECTOR rotation) { XMStoreFloat4x4(&_rotation, XMMatrixRotationRollPitchYawFromVector(rotation)); };
	XMFLOAT4X4 GetRotation() { return _rotation; };

	///
	// Scale Helpers!
	///
	void AddScale(XMVECTOR scale) {
		XMMATRIX bufferScale = XMLoadFloat4x4(&GetScale());
		XMStoreFloat4x4(&_scale, XMMatrixScalingFromVector(scale) * bufferScale);
	};

	void SetScale(XMVECTOR scale) { XMStoreFloat4x4(&_scale, XMMatrixScalingFromVector(scale)); };
	XMFLOAT4X4 GetScale() { return _scale; };

	XMFLOAT4X4 GetWorld() { return _world; };


	void SetID(UINT id) { _id = id; };
};
