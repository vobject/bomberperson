#ifndef SPRITE_RESOURCE_HPP
#define SPRITE_RESOURCE_HPP

#include "../../game/EntityId.hpp"
#include "../../utils/Utils.hpp"

#include <string>
#include <vector>

struct SDL_Surface;

// TODO: Make this the base class for PlayerResource.
//  First it has to adapt the animation technique that PlayerResource
//  is already using.
class SpriteResource
{
public:
   SpriteResource(EntityId id,
                  const std::vector<SDL_Surface*>& textures,
                  int animation_length = 0_ms,
                  bool looping = false);
   ~SpriteResource();

   SpriteResource(const SpriteResource&) = default;
   SpriteResource& operator=(const SpriteResource&) = delete;

   EntityId GetId() const;

   SDL_Surface* GetFrame(int anim_time = 0_ms) const;

private:
   const EntityId mId;
   const std::vector<SDL_Surface*> mFrames;
   const int mAnimationLength;
   const bool mLooping;
   int mMsPerFrame;
};

#endif // SPRITE_RESOURCE_HPP
