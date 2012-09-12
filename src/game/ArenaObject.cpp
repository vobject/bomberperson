#include "ArenaObject.hpp"

ArenaObject::ArenaObject(
   EntityId id,
   ZOrder zorder,
   const std::shared_ptr<Arena>& arena
)
   : SceneObject(id, zorder)
   , mArena(arena)
{

}

ArenaObject::~ArenaObject()
{

}

std::shared_ptr<Arena> ArenaObject::GetArena() const
{
   return mArena;
}
