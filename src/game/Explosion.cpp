#include "Explosion.hpp"
#include "../Options.hpp"

Explosion::Explosion(
   const std::shared_ptr<Arena>& arena,
   const ExplosionType type,
   const std::shared_ptr<Player>& owner
)
   : ArenaObject(EntityId::Explosion, ZOrder::Layer_5, arena)
   , mType(type)
   , mOwner(owner)
{
   if (ExplosionType::Center == mType) {
      mSound = ExplosionSound::Booom;
   }
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

ExplosionType Explosion::GetType() const
{
   return mType;
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
