#pragma once

#include <string>
#include "SDL.h"
#include <map>

class TextureManager
{
public:
	/**
	 * Gives access to the class, returns instance of this class.
	 * 
	 * \return s_Instanse
	 */
	static TextureManager* GetInstance()
	{
		//if already created -> return instance, if not -> create instance
		return s_Instanse = (s_Instanse != nullptr) ? s_Instanse : new TextureManager();
	}
	/**
	 * Load texture for object.
	 */
	bool Load(std::string id, std::string filename);
	/**
	 * Delete texture from object.
	 */
	void Drop(std::string id);
	/**
	 * Delete texture from memory.
	 */
	void Clean();
	/**
	 *  Draw the static texture (for objects, that don't have an anination).
	 * 
	 * \param id - id of the texture
	 * \param x - position of the texture 
	 * \param y 
	 * \param width - size of the texture
	 * \param height
	 * \param flip
	 */
	void Draw(std::string id, int x, int y, int width, int height, SDL_RendererFlip flip=SDL_FLIP_NONE);
	/**
	 * Draw the animation texture.
	 */
	void DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip); // width = width of the one frame
	bool ParseTextures(const char* source);

private:
	TextureManager() = default;
	static TextureManager* s_Instanse;
	std::map<std::string, SDL_Texture*> m_TextureMap;
};
