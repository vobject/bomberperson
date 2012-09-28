#include "Explosion.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"
#include "../Options.hpp"

Explosion::Explosion(
   const std::shared_ptr<Arena>& arena,
   const ExplosionType type,
   const PlayerType owner,
   EventQueue& queue
)
   : ArenaObject(EntityId::Explosion, ZOrder::Layer_5, arena)
   , mType(type)
   , mOwner(owner)
   , mEventQueue(queue)
{
   mEventQueue.Register(this);
}

Explosion::~Explosion()
{
   mEventQueue.UnRegister(this);
}

void Explosion::Update(const int elapsed_time)
{
   SetAnimationTime(GetAnimationTime() + elapsed_time);

   if ((ExplosionAnimation::Spawn == mAnimation) &&
       (GetAnimationTime() >= DefaultValue::EXPLOSION_SPAWN_ANIM_LEN))
   {
      // The explosion was just done spawning.
      mEventQueue.Add(std::make_shared<SpawnExplosionEndEvent>(GetInstanceId()));
   }

   if ((ExplosionAnimation::Burn == mAnimation) &&
       (GetAnimationTime() >= DefaultValue::EXPLOSION_BURN_ANIM_LEN))
   {
      mEventQueue.Add(std::make_shared<DestroyExplosionStartEvent>(GetInstanceId()));
   }

   if ((ExplosionAnimation::Destroy == mAnimation) &&
       (GetAnimationTime() >= DefaultValue::EXPLOSION_DESTROY_ANIM_LEN))
   {
      // The explosion was just done burning out.
      mEventQueue.Add(std::make_shared<DestroyExplosionEndEvent>(GetInstanceId()));
   }
}

void Explosion::OnEvent(const Event& event)
{
   if (event.GetSender() != GetInstanceId()) {
      // Explosions process only events that they send themself.
      return;
   }

   switch (event.GetType())
   {
      case EventType::SpawnExplosionStart:
         OnSpawnExplosionStart(dynamic_cast<const SpawnExplosionStartEvent&>(event));
         break;
      case EventType::SpawnExplosionEnd:
         OnSpawnExplosionEnd(dynamic_cast<const SpawnExplosionEndEvent&>(event));
         break;
      case EventType::DestroyExplosionStart:
         OnDestroyExplosionStart(dynamic_cast<const DestroyExplosionStartEvent&>(event));
         break;
      case EventType::DestroyExplosionEnd:
         OnDestroyExplosionEnd(dynamic_cast<const DestroyExplosionEndEvent&>(event));
         break;
      default:
         break;
   }
}

ExplosionType Explosion::GetType() const
{
   return mType;
}

ExplosionAnimation Explosion::GetAnimation() const
{
   return mAnimation;
}

PlayerType Explosion::GetOwner() const
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

void Explosion::OnSpawnExplosionStart(const SpawnExplosionStartEvent& event)
{
   SetAnimationTime(0);
   mAnimation = ExplosionAnimation::Spawn;

   if (ExplosionType::Center == mType) {
      mSound = ExplosionSound::Booom;
   }

   SetVisible(true);
}

void Explosion::OnSpawnExplosionEnd(const SpawnExplosionEndEvent& event)
{
   SetAnimationTime(0);
   mAnimation = ExplosionAnimation::Burn;
}

void Explosion::OnDestroyExplosionStart(const DestroyExplosionStartEvent& event)
{
   SetAnimationTime(0);
   mAnimation = ExplosionAnimation::Destroy;
}

void Explosion::OnDestroyExplosionEnd(const DestroyExplosionEndEvent& event)
{
   SetVisible(false);
   Invalidate();

   mEventQueue.Add(std::make_shared<RemoveExplosionEvent>(event.GetSender()));
}
