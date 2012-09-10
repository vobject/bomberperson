#include "Explosion.hpp"

Explosion::Explosion()
   : SceneObject(EntityId::Explosion)
{
   SetZOrder(ZOrder::Layer_6);
}

Explosion::~Explosion()
{

}

void Explosion::Update(const int elapsed_time)
{
   if (mSoundProcessed) {
      SetSound(SoundId::None);
   }

   mLifeTime += elapsed_time;

   if (mLifeTime >= DEFAULT_LIFETIME)
   {
      // The explosion has burned out.
      SetAlive(false);
   }
   mSoundProcessed = true;
}

int Explosion::GetAnimationTime() const
{
   return mLifeTime;
}
