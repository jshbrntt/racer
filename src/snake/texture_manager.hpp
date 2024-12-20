#pragma once
#include "game.hpp"
#include "engine/color.hpp"

class TextureManager
{
public:
	static void SetDrawColor(Color color);
	static SDL_Texture* LoadTexture(std::string texturePath);
	static void Draw(SDL_Texture* texture, SDL_Rect& src, SDL_Rect& dst);
	static void Draw(SDL_Texture * texture, SDL_Rect& src, SDL_Rect& dst, double rotation);
	static void Draw(TTF_Font* font, std::string text, SDL_Color color, int xOffset, int yOffset);
};