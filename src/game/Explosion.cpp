#include "Explosion.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"

Explosion::Explosion(
   const std::shared_ptr<Arena>& arena,
   const ExplosionType type,
   const int lifespan,
   const PlayerType owner,
   EventQueue& queue
)
   : ArenaObject(EntityId::Explosion, ZOrder::Layer_5, arena)
   , mType(type)
   , mLifeSpan(lifespan)
   , mOwner(owner)
   , mEventQueue(queue)
{
   mEventQueue.Register(this);

   if (ExplosionType::Center == mType) {
      mSound = ExplosionSound::Booom;
   }
}

Explosion::~Explosion()
{
   mEventQueue.UnRegister(this);
}

void Explosion::Update(const int elapsed_time)
{
   SetAnimationTime(GetAnimationTime() + elapsed_time);

   if (GetAnimationTime() >= mLifeSpan)
   {
      // The explosion has burned out.
      const auto parent_cell = GetArena()->GetCellFromObject(*this);
      mEventQueue.Add(std::make_shared<RemoveExplosionEvent>(GetInstanceId(),
                                                             parent_cell));
   }
}

void Explosion::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::RemoveExplosion:
         OnRemoveExplosion(dynamic_cast<const RemoveExplosionEvent&>(event));
         break;
      default:
         break;
   }
}

ExplosionType Explosion::GetType() const
{
   return mType;
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

void Explosion::OnRemoveExplosion(const RemoveExplosionEvent& event)
{
   const auto parent_cell = GetArena()->GetCellFromObject(*this);

   if (event.GetCell() != parent_cell) {
      return;
   }

   Invalidate();
}
