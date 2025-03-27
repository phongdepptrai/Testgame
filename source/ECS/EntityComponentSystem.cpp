#include "EntityComponentSystem.hpp"

void Entity::destroy() { active = false; }

void Entity::addGroup(Group mGroup) noexcept { groupBitSet[mGroup] = true; manager.AddToGroup(this, mGroup); }
// #include "EntityComponentSystem.hpp"
