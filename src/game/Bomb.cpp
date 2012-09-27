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

   mSound = BombSound::Planted;
}

Bomb::~Bomb()
{
   mEventQueue.UnRegister(this);
}

void Bomb::Update(const int elapsed_time)
{
   SetAnimationTime(GetAnimationTime() + elapsed_time);

   if (BombType::Remote != mType) {
      // The lifetime of a remote controlled bomb does not decline over time.
      mLifeTime += elapsed_time;
   }

   // TODO: Why is the Update() method called for an invalid bomb object?!
   if (IsValid() && (mLifeTime >= DefaultValue::BOMB_ANIM_LEN))
   {
      // ... instead it creates some explosions around it.
      PlantCenterExplosion();
      PlantRangeExplosion(Direction::Up);
      PlantRangeExplosion(Direction::Down);
      PlantRangeExplosion(Direction::Left);
      PlantRangeExplosion(Direction::Right);
      return;
   }

   if (mIsMoving)
   {
      UpdateMovement(elapsed_time);
   }
}

void Bomb::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::MoveBomb:
         OnMoveBomb(dynamic_cast<const MoveBombEvent&>(event));
         break;
      case EventType::CreateExplosion:
         OnCreateExplosion(dynamic_cast<const CreateExplosionEvent&>(event));
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

void Bomb::OnMoveBomb(const MoveBombEvent& event)
{
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
          up += (mSpeed * mDistance);
          break;
       case Direction::Down:
          down += (mSpeed * mDistance);
          break;
       case Direction::Left:
          left += (mSpeed * mDistance);
          break;
       case Direction::Right:
          right += (mSpeed * mDistance);
          break;
    }

    SetPosition({ GetPosition().X - left + right, GetPosition().Y - up + down});
    mMoveIdleTime = 0_ms;
}

void Bomb::OnCreateExplosion(const CreateExplosionEvent& event)
{
   const auto parent_cell = GetArena()->GetCellFromObject(*this);

   if ((event.GetCell() == parent_cell) &&
       (event.GetExplosionType() == ExplosionType::Center))
   {
      // Lifetime of this bomb object ended...
      Invalidate();
   }
}

void Bomb::OnDetonateRemoteBomb(const DetonateRemoteBombEvent& event)
{
   if (event.GetOwner() != mOwner) {
      // This event is not for us.
   }

   if (BombType::Remote != mType) {
      // This object is not a remote bomb.
   }

   // This bomb is a remote bomb and belongs to the player
   //  who requested his remotes to detonate. Do so (indirectly).
   mLifeTime = DefaultValue::BOMB_ANIM_LEN;
}

void Bomb::UpdateMovement(const int elapsed_time)
{
    mMoveIdleTime += elapsed_time;
    if (mMoveIdleTime < mSpeed) {
       return;
    }

    auto distance = 0;
    if (CanMove(mDirection, mDistance)) {
        distance = mDistance;
    }

    // If CanMove() was successful, the bomb will be moved.
    // But if CanMove() failed, the requested movement distance is 0. This means
    //  that the MoveBombEvent will cause the bomb to stop.

    mEventQueue.Add(std::make_shared<MoveBombEvent>(GetInstanceId(), mSpeed, distance, mDirection));
}

void Bomb::PlantCenterExplosion() const
{
   const auto parent_cell = GetArena()->GetCellFromObject(*this);

   mEventQueue.Add(std::make_shared<CreateExplosionEvent>(parent_cell,
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
         arena->GetBomb(range_cell)->mLifeTime = DefaultValue::BOMB_ANIM_LEN;
         break;
      }

      const auto exp_type = GetExplosionType(dir, (range_to_go == 1));

      mEventQueue.Add(std::make_shared<CreateExplosionEvent>(range_cell,
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
   if ((-1 != neighbor_cell.X) &&
       (-1 != neighbor_cell.Y) &&
       !arena->HasWall(neighbor_cell) &&
       !arena->HasBomb(neighbor_cell))
   {
      // A cell exists and does not block the bomb.
      return true;
   }
   return false;
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
