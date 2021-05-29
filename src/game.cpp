#include "game.hpp"

Game::Game() {}
Game::~Game() {}

void Game::init(const char *title,
                int x,
                int y,
                int width,
                int height,
                bool fullscreen)
{
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        SDL_Log("SDL Initialized");

        window = SDL_CreateWindow(title, x, y, width, height, flags);
        if (window)
        {
            SDL_Log("SDL Window Created");
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_Log("SDL Renderer Created");
        }

        isRunning = true;
    }
    else
    {
        isRunning = false;
    }
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;

    default:
        break;
    }
}

void Game::update() {}

void Game::render() {
    SDL_RenderClear(renderer);
    // TODO: Render
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    SDL_Log("Game Cleaned");
}
