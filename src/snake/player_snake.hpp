#pragma once

#include <entt/entity/registry.hpp>
#include "SDL.hpp"
#include "components/collider.hpp"

void processInput(entt::registry& registry, SDL_Event& event);
bool updateSnake(entt::registry& registry);
void moveSnake(entt::registry& registry);
void destroySnake(entt::registry& registry);
