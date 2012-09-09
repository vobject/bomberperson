#include "Explosion.hpp"

Explosion::Explosion()
   : SceneObject(EntityId::Explosion)
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
      SetAlive(false);
   }
}

int Explosion::GetAnimationTime() const
{
   return mLifeTime;
}
