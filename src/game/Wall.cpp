#include "Wall.hpp"

Wall::Wall(const std::shared_ptr<Arena>& arena, const WallType type)
   : ArenaObject(EntityId::Wall, ZOrder::Layer_3, arena)
   , mType(type)
{

}

Wall::~Wall()
{

}

void Wall::Update(const int elapsed_time)
{
   (void) elapsed_time;
}

WallType Wall::GetType() const
{
   return mType;
}

bool Wall::IsDestructible() const
{
   switch (mType)
   {
      case WallType::Indestructible:
         return false;
      case WallType::Destructible:
         return true;
   }
   return false;
}
