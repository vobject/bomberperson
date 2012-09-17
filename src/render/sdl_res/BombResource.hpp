#ifndef BOMB_RESOURCE_HPP
#define BOMB_RESOURCE_HPP

#include <vector>

enum class BombType;

struct SDL_Surface;

class BombResource
{
public:
   BombResource(BombType type, int anim_length, const std::vector<SDL_Surface*>& textures);
   ~BombResource();

   BombResource(const BombResource&) = default;
   BombResource& operator=(const BombResource&) = delete;

   BombType GetType() const;

   SDL_Surface* GetFrame(int anim_time) const;

private:
   const BombType mType;
   const std::vector<SDL_Surface*> mFrames;
   int mMsPerFrame;
};

#endif // BOMB_RESOURCE_HPP
