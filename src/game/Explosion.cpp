#include "Explosion.hpp"
#include "../Options.hpp"

Explosion::Explosion(const std::shared_ptr<Arena>& arena)
   : ArenaObject(EntityId::Explosion, ZOrder::Layer_6, arena)
{

}

Explosion::~Explosion()
{

}

void Explosion::Update(const int elapsed_time)
{
   SetAnimationTime(GetAnimationTime() + elapsed_time);

   if (GetAnimationTime() >= DefaultValue::EXPLOSION_ANIM_LENGTH)
   {
      // The explosion has burned out.
      Invalidate();
   }
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
