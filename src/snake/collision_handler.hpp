#pragma once

#include <entt/entity/registry.hpp>
#include "components/components.hpp"

void updateColliders(entt::registry& registry);
void checkPlayerCollision(entt::registry& registry);