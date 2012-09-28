#include "PlayerResource.hpp"
#include "../../game/Player.hpp"

#include <SDL.h>

PlayerResource::PlayerResource(const PlayerType type)
   : mType(type)
{

}

PlayerResource::~PlayerResource()
{

}

PlayerType PlayerResource::GetType() const
{
   return mType;
}

void PlayerResource::SetFrames(
   const PlayerAnimation anim,
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
      throw "PlayerResource: Invalid sprite textures.";
   }

   mFrames[anim] = { textures, length };
}

SDL_Surface* PlayerResource::GetFrame(
   const PlayerAnimation anim,
   const int anim_time,
   const int speed) const
{
   const auto iter = mFrames.find(anim);
   if (iter == mFrames.end()) {
      throw "Trying to access non-existing frame";
   }

   // An animation must always consist of one frame or more.
   // An animations length must always be > 0.

   const auto textures = iter->second.first;
   const auto original_anim_len = iter->second.second;

   const auto anim_len = original_anim_len - (speed * original_anim_len * .05f);
   const auto ms_per_frame = anim_len / textures.size();
   const int current_frame_index = anim_time / ms_per_frame;
   return textures.at(current_frame_index % textures.size());
}
