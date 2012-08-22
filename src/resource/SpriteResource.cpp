#include "SpriteResource.hpp"

#include <SDL.h>

SpriteResource::SpriteResource()
{

}

SpriteResource::SpriteResource(
   const std::string& id,
   const std::vector<SDL_Surface*>& textures
)
   : mId(id)
   , mFrames(textures)
{

}

SpriteResource::~SpriteResource()
{

}

std::string SpriteResource::GetId() const
{
   return mId;
}

int SpriteResource::GetFrameCount() const
{
   return mFrames.size();
}

SDL_Surface* SpriteResource::GetFrame(const int n) const
{
   return mFrames.at(n);
}

Size SpriteResource::GetSize() const
{
   if (mFrames.empty()) {
      throw "No frames associated with the current sprite";
   }
   // All frames ought to have the exact same size.
   return { mFrames.at(0)->w, mFrames.at(0)->h };
}
