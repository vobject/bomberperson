#ifndef SPRITE_RESOURCE_HPP
#define SPRITE_RESOURCE_HPP

#include "../game/EntityId.hpp"
#include "../utils/Utils.hpp"

#include <string>
#include <vector>

struct SDL_Surface;

// TODO: Make this the base class for PlayerResource.
//  First it has to adapt the animation technique that PlayerResource
//  is already using.
class SpriteResource
{
public:
   SpriteResource(EntityId id, const std::vector<SDL_Surface*>& textures);
   virtual ~SpriteResource();

   SpriteResource(const SpriteResource&) = default;
   SpriteResource& operator=(const SpriteResource&) = default;

   EntityId GetId() const;

   int GetFrameCount() const;
   SDL_Surface* GetFrame(int n) const;
//   Size GetSize() const;

private:
   const EntityId mId;
   std::vector<SDL_Surface*> mFrames;
};

#endif // SPRITE_RESOURCE_HPP
