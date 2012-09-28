#ifndef EXPLOSION_RESOURCE_HPP
#define EXPLOSION_RESOURCE_HPP

#include <map>
#include <vector>

enum class ExplosionType;
enum class ExplosionAnimation;

struct SDL_Surface;

class ExplosionResource
{
public:
   ExplosionResource(ExplosionType type);
   ~ExplosionResource();

   ExplosionResource(const ExplosionResource&) = default;
   ExplosionResource& operator=(const ExplosionResource&) = delete;

   ExplosionType GetType() const;

   SDL_Surface* GetFrame(ExplosionAnimation anim, int anim_time) const;
   void SetFrames(ExplosionAnimation anim, int length,
                  const std::vector<SDL_Surface*>& textures);

private:
   const ExplosionType mType;
   std::map<ExplosionAnimation, std::pair<std::vector<SDL_Surface*>, int>> mFrames;
};

#endif // EXPLOSION_RESOURCE_HPP
