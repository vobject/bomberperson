#ifndef PLAYER_RESOURCE_HPP
#define PLAYER_RESOURCE_HPP

#include <map>
#include <string>
#include <vector>

enum class PlayerType;
enum class PlayerAnimation;
struct SDL_Surface;

class PlayerResource
{
public:
   // TODO: Let each animation have another length.
   PlayerResource(PlayerType type);
   virtual ~PlayerResource();

   PlayerResource(const PlayerResource&) = default;
   PlayerResource& operator=(const PlayerResource&) = delete;

   PlayerType GetType() const;

   SDL_Surface* GetFrame(PlayerAnimation state, int anim_time, int speed) const;
   void SetFrames(PlayerAnimation state, int length, const std::vector<SDL_Surface*>& textures);

private:
   const PlayerType mType;
   std::map<PlayerAnimation, std::pair<std::vector<SDL_Surface*>, int>> mFrames;
};

#endif // PLAYER_RESOURCE_HPP
