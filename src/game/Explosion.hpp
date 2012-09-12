#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "ArenaObject.hpp"
#include "../utils/Utils.hpp"

//enum class ExplosionType
//{

//};

//enum class ExplosionSound
//{

//};

class Explosion : public ArenaObject
{
public:
   Explosion(const std::shared_ptr<Arena>& arena);
   virtual ~Explosion();

   Explosion(const Explosion&) = delete;
   Explosion& operator=(const Explosion&) = delete;

   void Update(int elapsed_time) override;

   int GetAnimationTime() const;

private:
   static const int DEFAULT_LIFETIME = 1000_ms;

   int mLifeTime = 0_ms;
//   bool mSoundProcessed = false;
};

#endif // EXPLOSION_HPP
