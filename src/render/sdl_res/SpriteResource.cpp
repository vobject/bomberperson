#include "SpriteResource.hpp"

#include <SDL.h>

SpriteResource::SpriteResource(
   const EntityId id,
   const std::vector<SDL_Surface*>& textures,
   const int animation_length /*=0_ms*/,
   const bool looping /*=false*/
)
   : mId(id)
   , mFrames(textures)
   , mAnimationLength(animation_length)
   , mLooping(looping)
{
   if (mFrames.empty()) {
      // An animation must always consist of at least one frame.
      throw "Invalid sprite textures.";
   }
   mMsPerFrame = mAnimationLength / mFrames.size();
}

SpriteResource::~SpriteResource()
{

}

EntityId SpriteResource::GetId() const
{
   return mId;
}

SDL_Surface* SpriteResource::GetFrame(const int anim_time /*=0_ms*/) const
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
