#ifndef EXPLOSION_RESOURCE_HPP
#define EXPLOSION_RESOURCE_HPP

#include <vector>

enum class ExplosionType;

struct SDL_Surface;

class ExplosionResource
{
public:
   ExplosionResource(ExplosionType type);
   ~ExplosionResource();

   ExplosionResource(const ExplosionResource&) = default;
   ExplosionResource& operator=(const ExplosionResource&) = delete;

   ExplosionType GetType() const;

   SDL_Surface* GetFrame(int anim_time) const;
   void SetFrames(int anim_length, const std::vector<SDL_Surface*>& textures);

private:
   const ExplosionType mType;
   std::vector<SDL_Surface*> mFrames;
   int mMsPerFrame = 0;
};

#endif // EXPLOSION_RESOURCE_HPP
