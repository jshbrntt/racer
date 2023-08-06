#pragma once

#include <iostream>
#include "SDL.h"

#if DEBUG == 1
#define LOG(x) SDL_Log(x);
#define LOG_ERROR(x) SDL_LogError(x);
#define LOG_SDL_ERROR() std::cout << "SDL_ERROR: " << SDL_GetError() << std::endl;
#else
#define LOG(x)
#define LOG_ERROR(x);
#define LOG_SDL_ERROR();
#endif