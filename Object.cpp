#include "Object.h"
#include "Structures.h"

using namespace DirectX;

Object::Object() {
	XMStoreFloat4x4(&_world, XMMatrixIdentity());
	XMStoreFloat4x4(&_position, XMMatrixIdentity());
	XMStoreFloat4x4(&_rotation, XMMatrixIdentity());
	XMStoreFloat4x4(&_scale, XMMatrixIdentity());

	_id = 0;
	_meshData = nullptr;
	_shader = nullptr;
	_texture = nullptr;
}

Object::~Object() {
	Destroy();
}

void Object::Destroy() {
	_meshData = nullptr;
	_shader = nullptr;
	_texture = nullptr;
}

void Object::LoadModel(MeshData* mesh) {
	_meshData = mesh;
}

void Object::LoadTexture(Texture* texture) {
	_texture = texture;
}

void Object::LoadShader(Shader* shader) {
	_shader = shader;
}

void Object::Update(double dt) {

	UpdateWorldCoords();
	return;
}

void Object::Draw(ID3D11DeviceContext* immediateContext, ID3D11Buffer* constantBuffer, XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, float t) {
	XMMATRIX world = XMLoadFloat4x4(&GetWorld());
	XMMATRIX view = XMLoadFloat4x4(&viewMatrix);
	XMMATRIX projection = XMLoadFloat4x4(&projectionMatrix);

	ConstantBuffer cb;
	cb.mWorld = XMMatrixTranspose(world);
	cb.mView = XMMatrixTranspose(view);
	cb.mProjection = XMMatrixTranspose(projection);
	cb.mTime = t;

	cb.mLightDirection = XMFLOAT3(0.25f, 1.0f, 0.25f);

	cb.mDiffuseLight = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	cb.mDiffuseMaterial = XMFLOAT4(0.8f, 0.5f, 0.5f, 1.0f);

	cb.mAmbientLight = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	cb.mAmbientMaterial = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	cb.mSpecularLight = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	cb.mSpecularMaterial = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	cb.mSpecularPower = 10.0f;
	cb.mEyePosW = XMFLOAT3(0.0f, 5.0f, -30.0f);

	ID3D11ShaderResourceView* texRSV = _texture->GetRSV(); 
	ID3D11SamplerState* sampleState = _shader->GetSS();
	
	immediateContext->VSSetShader(_shader->GetVS(), nullptr, 0);
	immediateContext->VSSetConstantBuffers(0, 1, &constantBuffer);
	immediateContext->PSSetConstantBuffers(0, 1, &constantBuffer);
	immediateContext->PSSetShader(_shader->GetPS(), nullptr, 0);
	immediateContext->PSSetSamplers(0, 1, &sampleState);


	immediateContext->PSSetShaderResources(0, 1, &texRSV);
	immediateContext->IASetVertexBuffers(0, 1, &_meshData->VertexBuffer, &_meshData->VBStride, &_meshData->VBOffset);
	immediateContext->IASetIndexBuffer(_meshData->IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	immediateContext->UpdateSubresource(constantBuffer, 0, nullptr, &cb, 0, 0);

	immediateContext->DrawIndexed(_meshData->IndexCount, 0, 0);
}

void Object::UpdateWorldCoords() {
	// TODO: find an alternative way to set world coordinates for the object

	XMMATRIX scale = XMLoadFloat4x4(&GetScale());
	XMMATRIX rotation = XMLoadFloat4x4(&GetRotation());
	XMMATRIX position = XMLoadFloat4x4(&GetPos());
	XMMATRIX world = XMLoadFloat4x4(&GetWorld());

	XMStoreFloat4x4(&_world, scale * position * rotation * world); // Apply local values before multiplying it into the world coordinates.

	XMStoreFloat4x4(&_scale, XMMatrixIdentity());
	XMStoreFloat4x4(&_rotation, XMMatrixIdentity());
	XMStoreFloat4x4(&_position, XMMatrixIdentity());
}