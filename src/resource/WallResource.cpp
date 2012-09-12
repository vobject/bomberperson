#include "WallResource.hpp"

WallResource::WallResource(
   const WallType type,
   const std::vector<SDL_Surface*>& textures
)
   : mType(type)
   , mFrames(textures)
{

}

WallResource::~WallResource()
{

}


WallType WallResource::GetType() const
{
   return mType;
}

SDL_Surface *WallResource::GetFrame() const
{
   return mFrames.at(0);
}
