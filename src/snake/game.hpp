#pragma once

#include "SDL.hpp"
#include "SDL_image.hpp"
#include "SDL_ttf.hpp"
#include <entt/entity/registry.hpp>

class Game
{
public:
	void initSDL(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void initEntt(entt::registry& registry);

	void handleEvents(entt::registry& registry);
	void update(entt::registry& registry);
	void render(entt::registry& registry);
	
	void cleanSDL();
	void cleanEntt(entt::registry& registry);

	bool running() const { return isRunning; }
	int frameCount() const { return frameCounter; }

	static SDL_Renderer* renderer;

private:
	bool isRunning;
	bool isAwaitingRestart;
	int frameCounter;
	SDL_Window* window;
	SDL_Event event;
};