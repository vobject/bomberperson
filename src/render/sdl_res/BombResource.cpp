#include "BombResource.hpp"
#include "../../game/Bomb.hpp"

BombResource::BombResource(const BombType type)
   : mType(type)
{

}

BombResource::~BombResource()
{

}


BombType BombResource::GetType() const
{
   return mType;
}

void BombResource::SetFrames(
      const int anim_length,
      const std::vector<SDL_Surface*>& textures
   )
{
   if (textures.empty()) {
      // An animation must always consist of at least one frame.
      throw "ExplosionResource: Invalid sprite textures.";
   }

   mFrames = textures;
   mMsPerFrame = anim_length / mFrames.size();
}

SDL_Surface* BombResource::GetFrame(const int anim_time) const
{
   if (!mMsPerFrame)
   {
      // An animations length of 0 indicates no animation.
      // The sprite has no animation but only one texture.
      return mFrames.at(0);
   }

   const int current_frame_index = anim_time / mMsPerFrame;
   return mFrames.at(current_frame_index % mFrames.size());
}
