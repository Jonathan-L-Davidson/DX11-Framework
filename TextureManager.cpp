#include "TextureManager.h"
#include <d3dcompiler.h>

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

HRESULT Shader::CompileShaderFromFile(WCHAR* szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** ppBlobOut)
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
	hr = D3DCompileFromFile(szFileName, nullptr, nullptr, szEntryPoint, szShaderModel,
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
/*
TextureManager::TextureManager(SDL_Renderer* renderer) {
	m_renderer = renderer;
	m_textures = new std::vector<Texture*>;
}

TextureManager::~TextureManager() {
	for (int i = 0; i < m_textures->size(); i++) {
		delete m_textures->at(i);
	}
	delete m_textures;
}

bool TextureManager::CreateTexture(std::string id, std::string cat) {
	Texture* tex = new Texture(id, cat, m_renderer);
	if (tex->GetTexture() == NULL) {
		delete tex;
		return false;
	}
	m_textures->push_back(tex);
	return true;
}

void TextureManager::RemoveTexture(std::string id) {
	for (int i = 0; i < m_textures->size(); i++) {
		if (id == m_textures->at(i)->GetID()) { // If ID is equal to the texture's ID.
			m_textures->erase(m_textures->begin() + i);
		}
	}
}

Texture* TextureManager::GetTexture(std::string id, std::string cat) {
	std::string null = "";

	//std::cout << "GetTexture(" << id << ", " << cat << ") started." << std::endl;

	Texture* result = new Texture(null, null, m_renderer);

	for (int i = 0; i < m_textures->size(); i++) {
		if (id == m_textures->at(i)->GetID() && cat == m_textures->at(i)->GetCategory()) { // If ID is equal to the texture's ID.
			//std::cout << "GetTexture() passed\n";
			result = m_textures->at(i);
		}
	}

	if (!result->GetID().size()) {
		std::cout << "Texture not found, creating new texture...\n";
		if (CreateTexture(id, cat))	result = GetTexture(id, cat);
	}

	return result;
}

void TextureManager::LoadTextures() {

}*/