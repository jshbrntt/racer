// FILE: "menu.cpp"

#include "menu.h"

SDL_Texture *Menu::menuTexture()
{
  // IMPORTING SPLASH SCREEN TEXTURE:
  return loadTexture("assets/menu/splashscreen.png");
}

Menu::Menu() : Entity(
                   getBox(S_WIDTH, S_HEIGHT),
                   getBox(S_WIDTH, S_HEIGHT),
                   Point(0, 0),
                   0,
                   menuTexture()) {}
