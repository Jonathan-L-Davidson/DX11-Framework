#include "TextureManager.h"
#include <filesystem>
#include <iostream>
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