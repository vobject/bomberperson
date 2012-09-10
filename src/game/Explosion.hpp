#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "SceneObject.hpp"
#include "../utils/Utils.hpp"

class Explosion : public SceneObject
{
public:
   Explosion();
   virtual ~Explosion();

   Explosion(const Explosion&) = delete;
   Explosion& operator=(const Explosion&) = delete;

   void Update(int elapsed_time) override;

   int GetAnimationTime() const;

private:
   static const int DEFAULT_LIFETIME = 1000_ms;

   int mLifeTime = 0_ms;
   bool mSoundProcessed = false;
};

#endif // EXPLOSION_HPP
