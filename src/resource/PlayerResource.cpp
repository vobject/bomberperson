#include "PlayerResource.hpp"

#include <SDL.h>

PlayerResource::PlayerResource(const EntityId id, const int animation_length /*=0_ms*/)
   : mId(id)
   , mAnimationLength(animation_length)
{

}

PlayerResource::~PlayerResource()
{

}

EntityId PlayerResource::GetId() const
{
   return mId;
}

//void PlayerResource::SetWalkFrames(
//   const Direction dir,
//   const std::vector<SDL_Surface*>& textures
//)
//{
//   mWalkFrames[dir] = textures;
//}

void PlayerResource::SetFrames(
   const PlayerAnimation state,
   const std::vector<SDL_Surface*>& textures
)
{
   mFrames[state] = textures;
}

//void PlayerResource::SetWalkDownFrames(const std::vector<SDL_Surface*>& textures_down)
//{
//   mFramesDown = textures_down;
//}
//
//void PlayerResource::SetWalkLeftFrames(const std::vector<SDL_Surface*>& textures_left)
//{
//   mFramesLeft = textures_left;
//}
//
//void PlayerResource::SetWalkRightFrames(const std::vector<SDL_Surface*>& textures_right)
//{
//   mFramesRight = textures_right;
//}

//int DirectedSpriteResource::GetFrameCount(const Direction dir) const
//{
//   switch (dir)
//   {
//      case Direction::Up:
//         return mFramesUp.size();
//      case Direction::Down:
//         return mFramesDown.size();
//      case Direction::Left:
//         return mFramesLeft.size();
//      case Direction::Right:
//         return mFramesRight.size();
//   }
//   return 0;
//}

//SDL_Surface* PlayerResource::GetWalkFrame(const Direction dir, const int n) const
//{
//   const auto iter = mWalkFrames.find(dir);
//   if (iter == mWalkFrames.end()) {
//      throw "Trying to access non-existing frame";
//   }
//   return iter->second.at(n);

////   switch (dir)
////   {
////      case Direction::Up:
////      case Direction::Down:
////         return mWalkFrames[dir].at(n);
////      case Direction::Left:
////         return mWalkFrames[dir].at(n);
////      case Direction::Right:
////         return mWalkFrames[dir].at(n);
////   }
////   return nullptr;
//}

SDL_Surface* PlayerResource::GetFrame(
   const PlayerAnimation state,
   const int anim_time,
   const int speed) const
{
   const auto iter = mFrames.find(state);
   if (iter == mFrames.end()) {
      throw "Trying to access non-existing frame";
   }

   // An animation must always consist of one frame or more.
   // An animations length must always be > 0.

   const auto anim_len = mAnimationLength - (speed * mAnimationLength * .05);
   const auto ms_per_frame = anim_len / iter->second.size();
   const int current_frame_index = anim_time / ms_per_frame;
   return iter->second.at(current_frame_index % iter->second.size());
}
