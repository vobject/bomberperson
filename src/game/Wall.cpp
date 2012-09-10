#include "Wall.hpp"

Wall::Wall(const EntityId wall_id)
   : SceneObject(wall_id)
{
   SetZOrder(ZOrder::Layer_3);
}

Wall::~Wall()
{

}

void Wall::Update(const int elapsed_time)
{

}

bool Wall::IsDestructible() const
{
   switch (GetId())
   {
      case EntityId::IndestructibleWall:
         return false;
      case EntityId::DestructibleWall:
         return true;
      default:
         return false;
   }
}
