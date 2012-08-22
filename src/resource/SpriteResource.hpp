#ifndef SPRITE_RESOURCE_HPP
#define SPRITE_RESOURCE_HPP

#include "../utils/Utils.hpp"

#include <string>
#include <vector>

struct SDL_Surface;

class SpriteResource
{
public:
   SpriteResource();
   SpriteResource(const std::string& id, const std::vector<SDL_Surface*>& textures);
   virtual ~SpriteResource();

   SpriteResource(const SpriteResource&) = default;
   SpriteResource& operator=(const SpriteResource&) = default;

   std::string GetId() const;
   int GetFrameCount() const;
   SDL_Surface* GetFrame(int n) const;
   Size GetSize() const;

private:
   std::string mId;
   std::vector<SDL_Surface*> mFrames;
};

#endif // SPRITE_RESOURCE_HPP
