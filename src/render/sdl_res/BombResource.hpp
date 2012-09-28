#ifndef BOMB_RESOURCE_HPP
#define BOMB_RESOURCE_HPP

#include <map>
#include <vector>

enum class BombType;
enum class BombAnimation;

struct SDL_Surface;

class BombResource
{
public:
   BombResource(BombType type);
   ~BombResource();

   BombResource(const BombResource&) = default;
   BombResource& operator=(const BombResource&) = delete;

   BombType GetType() const;

   SDL_Surface* GetFrame(BombAnimation anim, int anim_time) const;
   void SetFrames(BombAnimation anim, int length,
                  const std::vector<SDL_Surface*>& textures);

private:
   const BombType mType;
   std::map<BombAnimation, std::pair<std::vector<SDL_Surface*>, int>> mFrames;
};

#endif // BOMB_RESOURCE_HPP
