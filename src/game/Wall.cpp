#include "Wall.hpp"

Wall::Wall(const EntityId wall_id)
   : SceneObject(wall_id)
{

}

Wall::~Wall()
{

}

void Wall::Update(const int elapsed_time)
{

}

//WallType Wall::GetType() const
//{
//   return mType;
//}

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
