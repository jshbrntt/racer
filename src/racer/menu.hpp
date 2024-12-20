// FILE: "menu.hpp"

#include "global.hpp"

#ifndef MENU_H
#define MENU_H

class Menu : public Entity
{
private:
  SDL_Texture *menuTexture();

public:
  Menu();
};

#endif
