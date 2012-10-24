#ifndef ARENA_RESOURCE_HPP
#define ARENA_RESOURCE_HPP

#include <vector>

enum class ArenaType;

struct SDL_Surface;

// TODO: Add support for animations.

class ArenaResource
{
public:
   ArenaResource(ArenaType type, const std::vector<SDL_Surface*>& textures);
   ~ArenaResource();

   ArenaResource(const ArenaResource&) = default;
   ArenaResource& operator=(const ArenaResource&) = delete;

   ArenaType GetType() const;

   SDL_Surface* GetFrame() const;

private:
   const ArenaType mType;
   const std::vector<SDL_Surface*> mFrames;
};

#endif // ARENA_RESOURCE_HPP
