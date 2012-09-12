#include "Explosion.hpp"

Explosion::Explosion(const std::shared_ptr<Arena>& arena)
   : ArenaObject(EntityId::Explosion, ZOrder::Layer_6, arena)
{

}

Explosion::~Explosion()
{

}

void Explosion::Update(const int elapsed_time)
{
   mLifeTime += elapsed_time;

   if (mLifeTime >= DEFAULT_LIFETIME)
   {
      // The explosion has burned out.
      Invalidate();
   }
}

int Explosion::GetAnimationTime() const
{
   return mLifeTime;
}

ExplosionSound Explosion::GetSound(const bool reset)
{
   const auto ret = mSound;

   if (reset) {
      mSound = ExplosionSound::None;
   }
   return ret;
}

void Explosion::SetSound()
{
   mSound = ExplosionSound::Booom;
}
