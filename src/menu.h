// FILE: "menu.h"

#include "global.h"

#ifndef MENU_H
#define MENU_H

class Menu : public Entity
{
private:
  SDL_Surface *menuTexture();

public:
  Menu();
};

#endif
