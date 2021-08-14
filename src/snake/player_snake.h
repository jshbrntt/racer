#pragma once

#include <entt/entity/registry.hpp>
#include "SDL.h"
#include "components/collider.h"

void processInput(entt::registry& registry, SDL_Event& event);
bool updateSnake(entt::registry& registry);
void moveSnake(entt::registry& registry);
void destroySnake(entt::registry& registry);
