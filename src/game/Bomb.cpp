#include "Bomb.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"

#include "Arena.hpp"
#include "Wall.hpp"
#include "Extra.hpp"
#include "Explosion.hpp"
#include "../Options.hpp"

Bomb::Bomb(
   const std::shared_ptr<Arena>& arena,
   const BombType type,
   PlayerType owner,
   EventQueue& queue
)
   : ArenaObject(EntityId::Bomb, ZOrder::Layer_4, arena)
   , mType(type)
   , mOwner(owner)
   , mEventQueue(queue)
{
   mEventQueue.Register(this);
}

Bomb::~Bomb()
{
   mEventQueue.UnRegister(this);
}

void Bomb::Update(const int elapsed_time)
{
   SetAnimationTime(GetAnimationTime() + elapsed_time);

   if ((BombAnimation::Spawn == mAnimation) &&
       (GetAnimationTime() >= DefaultValue::BOMB_SPAWN_ANIM_LEN))
   {
      // The explosion was just done spawning.
      mEventQueue.Add(std::make_shared<SpawnBombEndEvent>(GetInstanceId()));
   }

   if (BombAnimation::Tick == mAnimation)
   {
      if (BombType::Remote != mType) {
         // The lifetime of a remote controlled bomb does not decline over time.
         mLifeTime += elapsed_time;
      }

      if (mLifeTime >= DefaultValue::BOMB_BURN_ANIM_LEN)
      {
         const auto parent_cell = GetArena()->GetCellFromObject(*this);
         mEventQueue.Add(std::make_shared<DestroyBombStartEvent>(GetInstanceId(),
                                                                 parent_cell));
      }
   }

   if ((BombAnimation::Destroy == mAnimation) &&
       (GetAnimationTime() >= DefaultValue::BOMB_DESTROY_ANIM_LEN))
   {
      const auto parent_cell = GetArena()->GetCellFromObject(*this);
      mEventQueue.Add(std::make_shared<DestroyBombEndEvent>(GetInstanceId(),
                                                            parent_cell,
                                                            mOwner));
   }

   UpdateMovement(elapsed_time);
}

void Bomb::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::SpawnBombStart:
         OnSpawnBombStart(dynamic_cast<const SpawnBombStartEvent&>(event));
         break;
      case EventType::SpawnBombEnd:
         OnSpawnBombEnd(dynamic_cast<const SpawnBombEndEvent&>(event));
         break;
      case EventType::DestroyBombStart:
         OnDestroyBombStart(dynamic_cast<const DestroyBombStartEvent&>(event));
         break;
      case EventType::DestroyBombEnd:
         OnDestroyBombEnd(dynamic_cast<const DestroyBombEndEvent&>(event));
         break;
      case EventType::MoveBomb:
         OnMoveBomb(dynamic_cast<const MoveBombEvent&>(event));
         break;
      case EventType::DetonateRemoteBomb:
         OnDetonateRemoteBomb(dynamic_cast<const DetonateRemoteBombEvent&>(event));
         break;
      default:
         break;
   }
}

BombType Bomb::GetType() const
{
   return mType;
}

BombAnimation Bomb::GetAnimation() const
{
   return mAnimation;
}

BombSound Bomb::GetSound(const bool reset)
{
   const auto ret = mSound;

   if (reset) {
      mSound = BombSound::None;
   }
   return ret;
}

int Bomb::GetRange() const
{
   return mRange;
}

void Bomb::SetRange(const int range)
{
    mRange = range;
}

void Bomb::OnSpawnBombStart(const SpawnBombStartEvent& event)
{
   if (event.GetSender() != GetInstanceId()) {
      return;
   }

   SetAnimationTime(0);
   mAnimation = BombAnimation::Spawn;
   mSound = BombSound::Planted;
   SetVisible(true);
}

void Bomb::OnSpawnBombEnd(const SpawnBombEndEvent& event)
{
   if (event.GetSender() != GetInstanceId()) {
      return;
   }

   SetAnimationTime(0);
   mAnimation = BombAnimation::Tick;
}

void Bomb::OnDestroyBombStart(const DestroyBombStartEvent& event)
{
   const auto parent_cell = GetArena()->GetCellFromObject(*this);

   if (event.GetCell() != parent_cell) {
      return;
   }

   SetAnimationTime(0);
   mAnimation = BombAnimation::Destroy;
}

void Bomb::OnDestroyBombEnd(const DestroyBombEndEvent& event)
{
   if (event.GetSender() != GetInstanceId()) {
      return;
   }

   SetVisible(false);
   Invalidate();

   const auto parent_cell = GetArena()->GetCellFromObject(*this);
   mEventQueue.Add(std::make_shared<RemoveBombEvent>(event.GetSender(),
                                                     parent_cell));

   PlantCenterExplosion();
   PlantRangeExplosion(Direction::Up);
   PlantRangeExplosion(Direction::Down);
   PlantRangeExplosion(Direction::Left);
   PlantRangeExplosion(Direction::Right);
}

void Bomb::OnMoveBomb(const MoveBombEvent& event)
{
   if (mMoveIdleTime < mSpeed) {
      return;
   }

   if (event.GetBombInstance() != GetInstanceId()) {
      // This event is not for us.
      return;
   }

   if (!event.GetDistance())
   {
      // This is a 'stop-moving' event.
      mIsMoving = false;
      return;
   }

   // This event may have been sent from a player who does not know
   //  if the bomb could actually be moved in the desired direction.
   //  So we have to check againg if everything is ok.
   if (!CanMove(event.GetDirection(), event.GetDistance()))
   {
      // Unable to move because of some obstacle.
      mIsMoving = false;
      return;
   }

   // This is needed in case the movement will change our parent cell.
   const auto old_parent_cell = GetArena()->GetCellFromObject(*this);

   // Initialize the class members with the movement information from the event.
   // We need this information because from now on the Bomb class will
   //  issue its own bomb movement events after the idle time passed.
   mSpeed = event.GetSpeed();
   mDistance = event.GetDistance();
   mDirection = event.GetDirection();

   auto up = 0;
   auto down = 0;
   auto left = 0;
   auto right = 0;

   switch (mDirection)
   {
      case Direction::Up:
         up += mDistance;
         break;
      case Direction::Down:
         down += mDistance;
         break;
      case Direction::Left:
         left += mDistance;
         break;
      case Direction::Right:
         right += mDistance;
         break;
   }

   SetPosition({ GetPosition().X - left + right, GetPosition().Y - up + down});
   mMoveIdleTime = 0_ms;
   mIsMoving = true;

   // The object has changed its parent cell due to the movement.
   const auto new_parent_cell = GetArena()->GetCellFromObject(*this);

   if (new_parent_cell != old_parent_cell)
   {
      mEventQueue.Add(std::make_shared<ParentCellChangedEvent>(GetInstanceId(),
                                                               old_parent_cell,
                                                               new_parent_cell));
   }
}

void Bomb::OnDetonateRemoteBomb(const DetonateRemoteBombEvent& event)
{
   if (event.GetOwner() != mOwner) {
      // This event is not for us.
      return;
   }

   if (BombType::Remote != mType) {
      // This object is not a remote bomb.
      return;
   }

   // This bomb is a remote bomb and belongs to the player
   //  who requested his remotes to detonate.
   SetAnimationTime(0);
   mAnimation = BombAnimation::Destroy;
}

void Bomb::UpdateMovement(const int elapsed_time)
{
    mMoveIdleTime += elapsed_time;
    if (mMoveIdleTime < mSpeed) {
       return;
    }

    // The receiving handler will not blindly execute the movement.
    // It will check if the movement is valid. If it is not, the
    //  bomb will stop moving.

    mEventQueue.Add(std::make_shared<MoveBombEvent>(GetInstanceId(),
                                                    GetInstanceId(),
                                                    mSpeed,
                                                    mDistance,
                                                    mDirection));
}

void Bomb::PlantCenterExplosion() const
{
   const auto parent_cell = GetArena()->GetCellFromObject(*this);

   mEventQueue.Add(std::make_shared<CreateExplosionEvent>(GetInstanceId(),
                                                          parent_cell,
                                                          ExplosionType::Center,
                                                          mOwner));
}

void Bomb::PlantRangeExplosion(const Direction dir) const
{
   const auto arena = GetArena();
   const auto parent = arena->GetCellFromObject(*this);
   auto range_cell = arena->GetNeighborCell(parent, dir);
   auto range_to_go = GetRange();

   while ((-1 != range_cell.X) && (-1 != range_cell.Y) && range_to_go)
   {
      if (arena->HasWall(range_cell) &&
          !arena->GetWall(range_cell)->IsDestructible())
      {
         // A wall that is not destructible is, well ... indestructible.
         // Do not spread the explosion in this direction any further.
         break;
      }

      if (arena->HasBomb(range_cell))
      {
         // A bombs explosion range ends if it hits another bomb in its way.
         // But it causes the other bomb to explode.
         mEventQueue.Add(std::make_shared<DestroyBombStartEvent>(GetInstanceId(),
                                                                 range_cell));
         break;
      }

      const auto exp_type = GetExplosionType(dir, (range_to_go == 1));

      mEventQueue.Add(std::make_shared<CreateExplosionEvent>(GetInstanceId(),
                                                             range_cell,
                                                             exp_type,
                                                             mOwner));

      if (arena->HasWall(range_cell) &&
          arena->GetWall(range_cell)->IsDestructible())
      {
         // FIXME: This has a minor problem: when the exploding bomb destroys
         //  a wall and triggers another bomb with a greater range,
         //  the wall behind the first wall would also be destroyed
         //  at the same time.
         // Maybe this could be avoided by introducing an ExplodingWall type.
         arena->DestroyWall(range_cell);
         break;
      }

      if (arena->HasExtra(range_cell))
      {
         // The explosion can destroy an extra item but it will be stopped
         //  when doing so.
         arena->GetExtra(range_cell)->Invalidate();
         break;
      }

      range_cell = arena->GetNeighborCell(range_cell, dir);
      range_to_go--;
   }
}

bool Bomb::CanMove(const Direction dir, const int distance) const
{
   // TODO: This code is basically copied from Player class.
   //  This might be a sign to outsource the whole collision detection
   //  stuff into its own class/subsytem.

   const auto arena = GetArena();
   const auto parent_cell = arena->GetCellFromObject(*this);
   const auto cell_size = arena->GetCellSize();
   const auto cell_pos = arena->GetCellPosition(parent_cell);

   // Check for movement inside the current cell.
   // Movement inside the current cell is always ok.
   switch (dir)
   {
      case Direction::Up:
         if ((GetPosition().Y - distance) >= cell_pos.Y)
            return true;
         break;
      case Direction::Down:
         if ((GetPosition().Y + GetSize().Height + distance) <= (cell_pos.Y + cell_size.Height))
            return true;
         break;
      case Direction::Left:
         if ((GetPosition().X - distance) >= cell_pos.X)
            return true;
         break;
      case Direction::Right:
         if ((GetPosition().X + GetSize().Width + distance) <= (cell_pos.X + cell_size.Width))
            return true;
         break;
   }

   // Bomb wants to move to another cell - check if that is allowed.
   const auto neighbor_cell = arena->GetNeighborCell(parent_cell, dir);

   if ((-1 == neighbor_cell.X) || (-1 == neighbor_cell.Y)) {
      // There is no cell in this direction.
      return false;
   }

   if (arena->HasWall(neighbor_cell) ||
       arena->HasBomb(neighbor_cell) ||
       arena->HasExtra(neighbor_cell))
   {
      // The path is blocked by a wall or a bomb.
      return false;
   }

   // The movement to another cell is permitted.
   return true;
}

ExplosionType Bomb::GetExplosionType(const Direction dir, const bool end) const
{
   switch (dir)
   {
      case Direction::Up:
         return end ? ExplosionType::VerticalUpEnd : ExplosionType::Vertical;
      case Direction::Down:
         return end ? ExplosionType::VerticalDownEnd : ExplosionType::Vertical;
      case Direction::Left:
         return end ? ExplosionType::HorizontalLeftEnd : ExplosionType::Horizontal;
      case Direction::Right:
         return end ? ExplosionType::HorizontalRightEnd : ExplosionType::Horizontal;
   }
   throw "Could not find the correct explosion type";
}
