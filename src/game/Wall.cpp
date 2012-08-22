#include "Wall.hpp"

Wall::Wall(const std::string& name, const WallType type)
   : mType(type)
{
   SetResourceId(name);
}

Wall::~Wall()
{

}

void Wall::Update(const int elapsed_time)
{

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
