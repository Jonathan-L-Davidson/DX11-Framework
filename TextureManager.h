#pragma once
#include <d3d11_1.h>

class Texture {
private:
	ID3D11ShaderResourceView* _texture;
public:
	UINT id;
	
	Texture();
	~Texture();

	ID3D11ShaderResourceView* GetTexture() { return _texture; };
};

class Shader {
private:
	ID3D11VertexShader* _vertexShader;
	ID3D11PixelShader* _pixelShader;
	ID3D11InputLayout* _vertexLayout;
	ID3D11SamplerState* _samplerLinear;
public:
	Shader();
	~Shader();

	void Destroy();

	HRESULT CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut);

	void SetShader(ID3D11DeviceContext* immediateContext);
};

//class TextureManager {
//	
//};

