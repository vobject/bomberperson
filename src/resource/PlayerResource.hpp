#ifndef PLAYER_RESOURCE_HPP
#define PLAYER_RESOURCE_HPP

#include <map>
#include <string>
#include <vector>

enum class Direction;
struct SDL_Surface;

class PlayerResource
{
public:
   PlayerResource();
   PlayerResource(const std::string& id);
   ~PlayerResource();

   PlayerResource(const PlayerResource&) = default;
   PlayerResource& operator=(const PlayerResource&) = default;

   std::string GetId() const;

   void SetWalkFrames(Direction dir, const std::vector<SDL_Surface*>& textures);
//   void SetWinFrames(const std::vector<SDL_Surface*>& textures_win);
//   void SetDeathFrames(const std::vector<SDL_Surface*>& textures_die);
//   int GetFrameCount(Direction dir) const;
   SDL_Surface* GetWalkFrame(Direction dir, int n) const;
//   SDL_Surface* GetWinFrame(Direction dir, int n) const;
//   SDL_Surface* GetDeathFrame(Direction dir, int n) const;
//   Size GetSize(Direction dir) const;

private:
   std::string mId;
   std::map<Direction, std::vector<SDL_Surface*>> mWalkFrames;
//   std::vector<SDL_Surface*> mFramesDown;
//   std::vector<SDL_Surface*> mFramesLeft;
//   std::vector<SDL_Surface*> mFramesRight;
//   std::vector<SDL_Surface*> mFramesWin;
//   std::vector<SDL_Surface*> mFramesDeath;
};

#endif // PLAYER_RESOURCE_HPP
