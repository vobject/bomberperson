#include "Explosion.hpp"

Explosion::Explosion(const std::string& name, const ExplosionType type)
   : mType(type)
{
   SetResourceId(name);

   mAnimation.SetFrameCount(4);
   mAnimation.SetLength(DEFAULT_LIFETIME);
   mAnimation.SetLooping(false);
}

Explosion::~Explosion()
{

}

void Explosion::Update(const int elapsed_time)
{
   mLifeTime += elapsed_time;

   if (mLifeTime >= DEFAULT_LIFETIME)
   {
      SetAlive(false);
   }

   mAnimation.Update(elapsed_time);
}

ExplosionType Explosion::GetType() const
{
   return mType;
}

int Explosion::GetAnimationFrame() const
{
   return mAnimation.GetCurrentFrame();
}
