#include "Explosion.hpp"
#include "../Options.hpp"

Explosion::Explosion(
   const std::shared_ptr<Arena>& arena,
   const std::shared_ptr<Player>& owner
)
   : ArenaObject(EntityId::Explosion, ZOrder::Layer_5, arena)
   , mOwner(owner)
{

}

Explosion::~Explosion()
{

}

void Explosion::Update(const int elapsed_time)
{
   SetAnimationTime(GetAnimationTime() + elapsed_time);

   if (GetAnimationTime() >= DefaultValue::EXPLOSION_ANIM_LEN)
   {
      // The explosion has burned out.
      Invalidate();
   }
}

std::shared_ptr<Player> Explosion::GetOwner() const
{
   return mOwner;
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
