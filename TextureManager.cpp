#include <d3dcompiler.h>
#include <stdio.h>
#include <string>
#include "TextureManager.h"



Shader::Shader() {
	_vertexShader = nullptr;
	_pixelShader = nullptr;
	_vertexLayout = nullptr;
	_samplerLinear = nullptr;
};

Shader::~Shader() {
	Destroy();
}

void Shader::Destroy() {
	if (_vertexShader) _vertexShader->Release();
	if (_pixelShader) _pixelShader->Release();
	if (_vertexLayout) _vertexLayout->Release();
	if (_samplerLinear) _samplerLinear->Release();
}

HRESULT Shader::Initialise(LPCWSTR name, ID3D11Device* device, ID3D11DeviceContext* immediateContext) {
	HRESULT hr = S_OK;

	// Define the input layout
	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	// Compile the vertex shader
	ID3DBlob* pVSBlob = nullptr;
	LPCWSTR* fileName = new LPCWSTR(name);

	hr = CompileShaderFromFile(fileName, "VS", "vs_4_0", &pVSBlob);

	if (FAILED(hr)) {
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the vertex shader
	hr = device->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &_vertexShader);

	if (FAILED(hr))	{
		pVSBlob->Release();
		return hr;
	}

	// Compile the pixel shader
	ID3DBlob* pPSBlob = nullptr;
	hr = CompileShaderFromFile(fileName, "PS", "ps_4_0", &pPSBlob);

	if (FAILED(hr))
	{
		MessageBox(nullptr,
			L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
		return hr;
	}

	// Create the pixel shader
	hr = device->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &_pixelShader);
	pPSBlob->Release();

	if (FAILED(hr)) {
		pPSBlob->Release();
		return hr;
	}

	UINT numElements = ARRAYSIZE(layout);

	// Create the input layout
	hr = device->CreateInputLayout(layout, numElements, pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), &_vertexLayout);
	pVSBlob->Release();

	if (FAILED(hr))
		return hr;

	// Set the input layout
	immediateContext->IASetInputLayout(_vertexLayout);

	delete fileName;
	return hr;
}


HRESULT Shader::CompileShaderFromFile(LPCWSTR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
{
	HRESULT hr = S_OK;

	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
	// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
	// Setting this flag improves the shader debugging experience, but still allows 
	// the shaders to be optimized and to run exactly the way they will run in 
	// the release configuration of this program.
	dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob;
	hr = D3DCompileFromFile(*szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
		dwShaderFlags, 0, ppBlobOut, &pErrorBlob);

	if (FAILED(hr))
	{
		if (pErrorBlob != nullptr)
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());

		if (pErrorBlob) pErrorBlob->Release();

		return hr;
	}

	if (pErrorBlob) pErrorBlob->Release();

	return S_OK;
}


HRESULT Shader::SetupSampler(ID3D11Device* device, ID3D11DeviceContext* immediateContext) {
	HRESULT hr;

	// Create the sample state
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = device->CreateSamplerState(&sampDesc, &_samplerLinear);

	if (FAILED(hr))
		return hr;

	immediateContext->PSSetSamplers(0, 1, &_samplerLinear);

	return S_OK;
};

Texture::Texture(LPCWSTR id) {
	_id = id;
	_textureRSV = nullptr;
}

Texture::~Texture() {
	delete _textureRSV;
}

Texture* Texture::LoadTexture(LPCWSTR FileName, ID3D11Device* device) {
	HRESULT result;
	result = CreateDDSTextureFromFile(device, FileName, nullptr, &_textureRSV);
	if (FAILED(result)) {
		throw;
	}
	return this;
}

TextureManager::TextureManager() {
	_textures = nullptr;
	_shaders = nullptr;

	_device = nullptr;
}

void TextureManager::Initialise() {
	_shaders = new std::vector<Shader*>();
	_textures = new std::vector<Texture*>();
}

TextureManager::~TextureManager() {
	_shaders->clear();
	delete _shaders;

	_textures->clear();
	delete _textures;
}

void TextureManager::CreateTexture(LPCWSTR id, LPCWSTR cat) {
	Texture* tex = new Texture(id);
	
	std::wstring extension = L".dds";
	std::wstring slash = L"/";
	std::wstring fileName = (std::wstring)cat + slash + (std::wstring)id + extension;
	
	tex = tex->LoadTexture(fileName.c_str(), _device->GetDevice());

	if (tex->GetTexture() == NULL) {
		delete tex;
		return;
	}
	_textures->push_back(tex);
}

void TextureManager::RemoveTexture(LPCWSTR id) {
	for (UINT i = 0; i < _textures->size(); i++) {
		if (id == _textures->at(i)->GetID()) { // If ID is equal to the texture's ID.
			_textures->erase(_textures->begin() + i);
		}
	}
}

Texture* TextureManager::GetTexture(LPCWSTR id) {
	
	for (UINT i = 0; i < _textures->size(); i++) {
		if (id == _textures->at(i)->GetID()) { // If ID is equal to the texture's ID.
			//std::cout << "GetTexture() passed\n";
			return _textures->at(i);
		}
	}

	LPCWSTR Category = L"Textures";

	CreateTexture(id, Category);
	return GetTexture(id); // Return the 1st texture loaded.
}

//void TextureManager::CreateShader(LPCWSTR id, LPCWSTR cat) {
//	Shader* shad = new Shader(id);
//
//	LPCWSTR fileName = cat + *L"/" + id + *L".dds";
//	shad->LoadShader(fileName, _device);
//
//	if (shad->GetShader() == NULL) {
//		delete shad;
//		return;
//	}
//	_textures->push_back(tex);
//}
//
//void TextureManager::RemoveShader(LPCWSTR id) {
//	for (int i = 0; i < _shaders->size(); i++) {
//		if (id == _shaders->at(i)->GetID()) { // If ID is equal to the texture's ID.
//			_shaders->erase(_shaders->begin() + i);
//		}
//	}
//}
//
//Shader* TextureManager::GetShader(LPCWSTR id) {
//
//	std::cout << "GetTexture(" << id << ", " << cat << ") started." << std::endl;
//
//	Shader* result;
//
//	for (int i = 0; i < _shaders->size(); i++) {
//		if (id == _shaders->at(i)->GetID()) { // If ID is equal to the texture's ID.
//			std::cout << "GetTexture() passed\n";
//			result = _shaders->at(i);
//		}
//	}
//	if (!result || !wcslen((LPCWSTR_t*)result->GetID())) { // For some reason, LPCWSTR is a typedefinition for LPCWSTR_t, YET IT'S NOT A LPCWSTR_T???
//		result = CreateShader(id);
//		return _shaders->at(0); // Return the 1st texture loaded.
//	}
//
//	return result;
//}
