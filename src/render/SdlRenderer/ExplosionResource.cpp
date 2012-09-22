#include "ExplosionResource.hpp"

ExplosionResource::ExplosionResource(
   const ExplosionType type,
   const int anim_length,
   const std::vector<SDL_Surface*>& textures
)
   : mType(type)
   , mFrames(textures)
{
   if (mFrames.empty()) {
      // An animation must always consist of at least one frame.
      throw "ExplosionResource: Invalid sprite textures.";
   }
   mMsPerFrame = anim_length / mFrames.size();
}

ExplosionResource::~ExplosionResource()
{

}


ExplosionType ExplosionResource::GetType() const
{
   return mType;
}

SDL_Surface *ExplosionResource::GetFrame(const int anim_time) const
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
