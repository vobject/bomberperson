#ifndef WALL_RESOURCE_HPP
#define WALL_RESOURCE_HPP

#include <vector>

enum class WallType;

struct SDL_Surface;

// TODO: Add support for wall destruction animations.

class WallResource
{
public:
   WallResource(WallType type, const std::vector<SDL_Surface*>& textures);
   ~WallResource();

   WallResource(const WallResource&) = default;
   WallResource& operator=(const WallResource&) = delete;

   WallType GetType() const;

   SDL_Surface* GetFrame() const;

private:
   const WallType mType;
   const std::vector<SDL_Surface*> mFrames;
};

#endif // WALL_RESOURCE_HPP
