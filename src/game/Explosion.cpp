#include "Explosion.hpp"

Explosion::Explosion()
   : SceneObject(EntityId::Explosion)
{
   mAnimation.SetFrameCount(4);
   mAnimation.SetLength(DEFAULT_LIFETIME);
   mAnimation.SetLooping(false);
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
      SetAlive(false);
   }

   mAnimation.Update(elapsed_time);

   mSoundProcessed = true;
}

int Explosion::GetAnimationFrame() const
{
   return mAnimation.GetCurrentFrame();
}
