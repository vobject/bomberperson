#ifndef PLAYER_RESOURCE_HPP
#define PLAYER_RESOURCE_HPP

#include "../game/EntityId.hpp"
#include "../utils/Utils.hpp"

#include <map>
#include <string>
#include <vector>

enum class PlayerState;
//enum class Direction;
struct SDL_Surface;

class PlayerResource
{
public:
   PlayerResource(EntityId id);
   virtual ~PlayerResource();

   PlayerResource(const PlayerResource&) = default;
   PlayerResource& operator=(const PlayerResource&) = default;

   EntityId GetId() const;

   SDL_Surface* GetFrame(PlayerState state, int state_time, int speed) const;
   void SetFrames(PlayerState state, const std::vector<SDL_Surface*>& textures);

//   void SetWalkFrames(Direction dir, const std::vector<SDL_Surface*>& textures);
//   void SetWinFrames(const std::vector<SDL_Surface*>& textures_win);
//   void SetDeathFrames(const std::vector<SDL_Surface*>& textures_die);
//   int GetFrameCount(Direction dir) const;
//   SDL_Surface* GetWalkFrame(Direction dir, int n) const;
//   SDL_Surface* GetWinFrame(Direction dir, int n) const;
//   SDL_Surface* GetDeathFrame(Direction dir, int n) const;
//   Size GetSize(Direction dir) const;

   // TODO: Let each animation have another length.
   void SetAnimationLength(int ms);
   // SetLooping(bool)

private:
   const EntityId mId;
//   std::map<Direction, std::vector<SDL_Surface*>> mWalkFrames;
   std::map<PlayerState, std::vector<SDL_Surface*>> mFrames;
   int mAnimationLength = 0_ms;
//   std::vector<SDL_Surface*> mFramesDown;
//   std::vector<SDL_Surface*> mFramesLeft;
//   std::vector<SDL_Surface*> mFramesRight;
//   std::vector<SDL_Surface*> mFramesWin;
//   std::vector<SDL_Surface*> mFramesDeath;
};

#endif // PLAYER_RESOURCE_HPP
