// FILE: "menu.h"

#include "global.h"

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