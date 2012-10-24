#include "ArenaResource.hpp"

ArenaResource::ArenaResource(
   const ArenaType type,
   const std::vector<SDL_Surface*>& textures
)
   : mType(type)
   , mFrames(textures)
{
   if (mFrames.empty()) {
      // An animation must always consist of at least one frame.
      throw "ArenaResource: Invalid sprite textures.";
   }
}

ArenaResource::~ArenaResource()
{

}


ArenaType ArenaResource::GetType() const
{
   return mType;
}

SDL_Surface *ArenaResource::GetFrame() const
{
   return mFrames.at(0);
}
