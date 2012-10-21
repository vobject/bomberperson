#ifndef BOMB_RESOURCE_HPP
#define BOMB_RESOURCE_HPP

#include <vector>

enum class BombType;

struct SDL_Surface;

class BombResource
{
public:
   BombResource(BombType type);
   ~BombResource();

   BombResource(const BombResource&) = default;
   BombResource& operator=(const BombResource&) = delete;

   BombType GetType() const;

   SDL_Surface* GetFrame(int anim_time) const;
   void SetFrames(int anim_length, const std::vector<SDL_Surface*>& textures);

private:
   const BombType mType;
   std::vector<SDL_Surface*> mFrames;
   int mMsPerFrame = 0;
};

#endif // BOMB_RESOURCE_HPP
