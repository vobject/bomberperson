#include "ExplosionResource.hpp"
#include "../../game/Explosion.hpp"

#include <SDL.h>

ExplosionResource::ExplosionResource(const ExplosionType type)
   : mType(type)
{

}

ExplosionResource::~ExplosionResource()
{

}


ExplosionType ExplosionResource::GetType() const
{
   return mType;
}

void ExplosionResource::SetFrames(
   const ExplosionAnimation anim,
   const int length,
   const std::vector<SDL_Surface*>& textures
)
{
   if (!length) {
      // We might divide by zero if the animation's length was 0.
      throw "The length of an animation cannot be 0.";
   }

   if (textures.empty()) {
      // An animation must always consist of at least one frame.
      throw "ExplosionResource: Invalid sprite textures.";
   }

   mFrames.insert({ anim, { textures, length }});
}

SDL_Surface* ExplosionResource::GetFrame(
   const ExplosionAnimation anim,
   const int anim_time
) const
{
   const auto iter = mFrames.find(anim);
   if (iter == mFrames.end()) {
      throw "Trying to access non-existing frame";
   }

   // An animation must always consist of one frame or more.
   // An animations length must always be > 0.

   const auto textures = iter->second.first;
   const auto anim_len = iter->second.second;

   const auto ms_per_frame = anim_len / textures.size();
   const int current_frame_index = anim_time / ms_per_frame;
   return textures.at(current_frame_index % textures.size());
}
