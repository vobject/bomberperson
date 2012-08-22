#include "PlayerResource.hpp"
#include "../utils/Utils.hpp"

#include <SDL.h>

PlayerResource::PlayerResource()
{

}

PlayerResource::PlayerResource(const std::string& id)
   : mId(id)
{

}

PlayerResource::~PlayerResource()
{

}

std::string PlayerResource::GetId() const
{
   return mId;
}

void PlayerResource::SetWalkFrames(
   const Direction dir,
   const std::vector<SDL_Surface*>& textures
)
{
   mWalkFrames[dir] = textures;
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

SDL_Surface* PlayerResource::GetWalkFrame(const Direction dir, const int n) const
{
   const auto iter = mWalkFrames.find(dir);
   if (iter == mWalkFrames.end()) {
      throw "Trying to access non-existing frame";
   }
   return iter->second.at(n);

//   switch (dir)
//   {
//      case Direction::Up:
//      case Direction::Down:
//         return mWalkFrames[dir].at(n);
//      case Direction::Left:
//         return mWalkFrames[dir].at(n);
//      case Direction::Right:
//         return mWalkFrames[dir].at(n);
//   }
//   return nullptr;
}

//Size DirectedSpriteResource::GetSize(const Direction dir) const
//{
//   switch (dir)
//   {
//      case Direction::Up:
//         return { mFramesUp.at(0)->w, mFramesUp.at(0)->h };
//      case Direction::Down:
//         return { mFramesDown.at(0)->w, mFramesDown.at(0)->h };
//      case Direction::Left:
//         return { mFramesLeft.at(0)->w, mFramesLeft.at(0)->h };
//      case Direction::Right:
//         return { mFramesRight.at(0)->w, mFramesRight.at(0)->h };
//   }
//   return { 0, 0 };
//}
