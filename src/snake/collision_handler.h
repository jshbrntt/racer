#pragma once

#include <entt/entity/registry.hpp>
#include "components/components.h"

void updateColliders(entt::registry& registry);
void checkPlayerCollision(entt::registry& registry);