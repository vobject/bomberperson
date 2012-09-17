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

//bool ArenaObject::IsDying() const
//{
//   return mIsDying;
//}

//void ArenaObject::StartDying()
//{
//   mIsDying = true;
//}

std::shared_ptr<Arena> ArenaObject::GetArena() const
{
   return mArena;
}
