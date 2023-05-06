#include "TextureManager.h"
#include "Engine.h"
#include <tinyxml2.h>
#include <string>
#include <iostream>

TextureManager* TextureManager::s_Instanse = nullptr;

bool TextureManager::Load(std::string id, std::string filename)
{
	SDL_Surface* surface = IMG_Load(filename.c_str());
	if (surface == nullptr)
	{
		SDL_Log("Failed to load texture: %s, %s", filename.c_str(), SDL_GetError());
		return false;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface);
	if (texture == nullptr)
	{
		SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
		return false;
	}

	m_TextureMap[id] = texture; // add texture to map
	
	return true;
}
/**
 * Delete one texture from memory, if it is no needed anymore.
 * 
 * \param id - id of the texture
 */
void TextureManager::Drop(std::string id)
{
	SDL_DestroyTexture(m_TextureMap[id]);
	m_TextureMap.erase(id);
}

/**
 * Delete all textures from the map.
 */
void TextureManager::Clean()
{
	std::map<std::string, SDL_Texture*>::iterator it;
	for (it = m_TextureMap.begin(); it != m_TextureMap.end(); it++)
	{
		SDL_DestroyTexture(it->second);
	}
	m_TextureMap.clear();

	SDL_Log("Texture map cleaned");
}

/**
 * SDL_Rect define which part of the image is showing on the screen.
 * Source - part of the image, that we want to show on the screen.
 * Destination - where on the screen we want to show image.
 * 
 * \param id
 * \param x
 * \param y
 * \param width
 * \param height
 * \param flip
 */
void TextureManager::Draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip)
{
	SDL_Rect srcRect = { 0, 0, width, height };
	SDL_Rect destRect = { x, y, width, height };
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &destRect, 0, nullptr, flip); // 0 - don't want to rotate
}

void TextureManager::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip)
{
	SDL_Rect srcRect = { width*frame, height*row, width, height };
	SDL_Rect destRect = { x, y, width, height };
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &destRect, 0, nullptr, flip); // 0 - don't want to rotate
}

/**
 * ParseTextures.
 * Accesses and interacts with XML-file, that holds animations.
 * Reduces code size, that makes the application faster to compile. 
 * Using: https://github.com/leethomason/tinyxml2
 * 
 * \param source - xml-file
 * \return 
 */
bool TextureManager::ParseTextures(const char* source)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(source);
	if (doc.Error())
	{
		std::cout << "Failed to load " << source << "\n";
		return false;
	}

	tinyxml2::XMLElement* root = doc.RootElement();
	for (tinyxml2::XMLElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("texture"))
		{
			std::string id = e->Attribute("id");
			std::string src = e->Attribute("source");
			Load(id, src);
		}
	}

	return true;
}
