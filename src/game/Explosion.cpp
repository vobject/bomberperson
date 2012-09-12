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
//   if (mSoundProcessed) {
//      SetSound(SoundId::None);
//   }

   mLifeTime += elapsed_time;

   if (mLifeTime >= DEFAULT_LIFETIME)
   {
      // The explosion has burned out.
      Invalidate();
   }
//   mSoundProcessed = true;
}

int Explosion::GetAnimationTime() const
{
   return mLifeTime;
}
