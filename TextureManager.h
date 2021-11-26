#pragma once
#include <d3d11_1.h>
#include <stdio.h>
#include <vector>
#include "DxDevice.h"
#include "TextureLoader.h"

using namespace DirectX;

class Texture {
private:
	DWORD _id;
	ID3D11ShaderResourceView* _textureRSV;

public:
	
	Texture(DWORD id);
	~Texture();


	Texture* LoadTexture(DWORD FileName, ID3D11Device* device);

	Texture* GetTexture() { return this; };
	DWORD GetID() { return _id; };
	ID3D11ShaderResourceView* GetRSV() { return _textureRSV; };
};

class Shader {
private:
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _vertexLayout;
	ID3D11SamplerState* _samplerLinear;

	HRESULT SetupSampler(ID3D11Device* device, ID3D11DeviceContext* immediateContext);

public:
	Shader();
	~Shader();

	void Destroy();

	HRESULT Initialise(DWORD* name, ID3D11Device* device, ID3D11DeviceContext* immediateContext);
	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);


	void SetShader(ID3D11DeviceContext* immediateContext);
};

class TextureManager {
private:
	DxDevice*				_device;

	std::vector<Shader*>*	_shaders;
	std::vector<Texture*>*	_textures;

public:
	TextureManager();
	~TextureManager();
	void Initialise();

	void CreateTexture(DWORD id, DWORD cat);
	void RemoveTexture(DWORD id);
	Texture* GetTexture(DWORD id);
	ID3D11ShaderResourceView* GetTextureRSV(DWORD id) { return GetTexture(id)->GetRSV(); };

	void CreateShader(DWORD id, DWORD cat);
	void RemoveShader(DWORD id);
	Shader* GetShader(DWORD id);

	void SetDevice(DxDevice* device) { _device = device; };
};

