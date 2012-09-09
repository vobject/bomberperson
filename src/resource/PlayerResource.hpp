#ifndef PLAYER_RESOURCE_HPP
#define PLAYER_RESOURCE_HPP

#include "../game/EntityId.hpp"
#include "../utils/Utils.hpp"

#include <map>
#include <string>
#include <vector>

enum class PlayerAnimation;
struct SDL_Surface;

class PlayerResource
{
public:
   // TODO: Let each animation have another length.
   PlayerResource(EntityId id, int animation_length = 0_ms);
   virtual ~PlayerResource();

   PlayerResource(const PlayerResource&) = default;
   PlayerResource& operator=(const PlayerResource&) = default;

   EntityId GetId() const;

   SDL_Surface* GetFrame(PlayerAnimation state, int anim_time, int speed) const;
   void SetFrames(PlayerAnimation state, const std::vector<SDL_Surface*>& textures);

private:
   const EntityId mId;
   const int mAnimationLength;
   std::map<PlayerAnimation, std::vector<SDL_Surface*>> mFrames;
};

#endif // PLAYER_RESOURCE_HPP
