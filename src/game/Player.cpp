#include "Player.hpp"
#include "EntityManager.hpp"
#include "Arena.hpp"
#include "Extra.hpp"
#include "Bomb.hpp"
#include "Explosion.hpp"
#include "../utils/Utils.hpp"

Player::Player(
   const std::shared_ptr<Arena>& arena,
   const PlayerType type,
   EntityManager& entity_factory
)
   : ArenaObject(EntityId::Player, ZOrder::Layer_5, arena)
   , mType(type)
   , mData(PlayerAnimation::StandDown, 0_ms, MIN_SPEED, 1, 1, 1, 0)
   , mEntityFactory(entity_factory)
{

}

Player::~Player()
{

}

void Player::Update(const int elapsed_time)
{
//   SetSound(SoundId::None);

   const auto old_anim = mData.anim;
   const auto parent_cell = GetArena()->GetCellFromObject(*this);

   LOG(logDEBUG) << "Cell(X=" << parent_cell.X << ",Y=" << parent_cell.Y << ")";

   if (GetArena()->HasExplosion(parent_cell))
   {
      // Explosions kill the player instantly.
//      SetSound(SoundId::PlayerDies);
      Invalidate();
      return;
   }

   if (GetArena()->HasExtra(parent_cell))
   {
      switch (GetArena()->GetExtra(parent_cell)->GetType())
      {
         case ExtraType::Speed:
            IncreaseSpeed();
            break;
         case ExtraType::Bombs:
            mData.bombs = std::min(mData.bombs + 1, 99);
            break;
         case ExtraType::Range:
            mData.range = std::min(mData.range + 1, 99);
            break;
         case ExtraType::InfiniteRange:
            mData.range = 99;
            break;
         default:
            break;
      }
      // An extra should no longer exist after it was picked
      //  up by a player.
      GetArena()->DestroyExtra(parent_cell);

//      SetSound(SoundId::PlayerPicksUpExtra);
   }

   UpdateMovement(elapsed_time);
   UpdateBombing(elapsed_time);

   if (old_anim == mData.anim) {
      mData.anim_time += elapsed_time;
   }
   else {
      mData.anim_time = 0; // Start new animation.
   }
}

//void Player::SetParentCell(const std::shared_ptr<Cell>& cell)
//{
//   mParentCell = cell;
//}

void Player::SetInputCommands(const InputCommands cmds)
{
   mCurrentCommands = cmds;
}

PlayerType Player::GetType() const
{
   return mType;
}

PlayerData Player::GetData() const
{
   return mData;
}

void Player::UpdateMovement(const int elapsed_time)
{
   mMoveIdleTime += elapsed_time;
   if (mMoveIdleTime < mData.speed) {
      return;
   }

   auto up = 0;
   auto down = 0;
   auto left = 0;
   auto right = 0;
   auto update_anim = false;

   if (mCurrentCommands.up)
   {
      update_anim = true;
      mData.anim = PlayerAnimation::WalkUp;

      if (CanMove(Direction::Up, mData.distance)) {
         up += mData.distance;
      }
   }
   if (mCurrentCommands.down)
   {
      update_anim = true;
      mData.anim = PlayerAnimation::WalkDown;

      if (CanMove(Direction::Down, mData.distance)) {
         down += mData.distance;
      }
   }
   if (mCurrentCommands.left)
   {
      update_anim = true;
      mData.anim = PlayerAnimation::WalkLeft;

      if (CanMove(Direction::Left, mData.distance)) {
         left += mData.distance;
      }
   }
   if (mCurrentCommands.right)
   {
      update_anim = true;
      mData.anim = PlayerAnimation::WalkRight;

      if (CanMove(Direction::Right, mData.distance)) {
         right += mData.distance;
      }
   }

   SetPosition({ GetPosition().X - left + right, GetPosition().Y - up + down});

   if (!update_anim)
   {
      mData.anim = GetStopWalkingState(mData.anim);
   }

   mMoveIdleTime = 0_ms;
}

void Player::UpdateBombing(const int elapsed_time)
{
   mBombIdleTime += elapsed_time;
   if (mBombIdleTime < mPlantingSpeed) {
      return;
   }

   if (!mCurrentCommands.action1 && !mCurrentCommands.action2)
   {
      // The user did not request to plant a bomb.
      return;
   }

   // TODO: Use Action2 input to detonate remote controlled bombs etc.

   const auto parent_cell = GetArena()->GetCellFromObject(*this);

   if (GetArena()->HasBomb(parent_cell)) {
      // Only one bomb allowed per cell.
      return;
   }

   if (!CanPlantBomb()) {
      // Out of bomb supply. Wait till an older bomb exploded.
      return;
   }

   auto bomb = mEntityFactory.CreateBomb(parent_cell);
   bomb->SetRange(mData.range);
//   bomb->SetOwner(GetId());
//   bomb->SetSound(SoundId::BombPlanted);
   GetArena()->SetBomb(parent_cell, bomb);

   mPlantedBombs.push_back(bomb);
   mBombIdleTime = 0_ms;
}

bool Player::CanMove(const Direction dir, const int distance) const
{
   const auto parent_cell = GetArena()->GetCellFromObject(*this);
   const auto cell_size = GetArena()->GetCellSize();
   const auto cell_pos = GetArena()->GetCellPosition(parent_cell);

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

   // Player wants to walk to another cell - check if that is allowed.
   const auto neighbor_cell = GetArena()->GetNeighborCell(parent_cell, dir);
   if ((-1 != neighbor_cell.X) &&
       (-1 != neighbor_cell.Y) &&
       !GetArena()->HasWall(neighbor_cell) &&
       !GetArena()->HasBomb(neighbor_cell))
   {
      // A cell exists and does not block the player.
      return true;
   }

   return false;
}

bool Player::CanPlantBomb()
{
   int bombs_alive = 0;
   for (const auto& bomb : mPlantedBombs)
   {
      if (bomb->IsValid())
      {
         bombs_alive++;
      }
   }

   // Garbage collection of the planted-bombs-vector.
   if (!bombs_alive) {
      mPlantedBombs.clear();
   }

   return (mData.bombs > bombs_alive);
}

void Player::IncreaseSpeed()
{
   if (mData.speed > MAX_SPEED)
   {
      mData.speed -= 2_ms;
   }
   else
   {
      // The player will now move an additional pixel per interval
      //  if he really got this fast.
      mData.distance++;
   }
}

PlayerAnimation Player::GetStopWalkingState(const PlayerAnimation anim) const
{
   switch (anim)
   {
      case PlayerAnimation::WalkUp:
         return PlayerAnimation::StandUp;
      case PlayerAnimation::WalkDown:
         return PlayerAnimation::StandDown;
      case PlayerAnimation::WalkLeft:
         return PlayerAnimation::StandLeft;
      case PlayerAnimation::WalkRight:
         return PlayerAnimation::StandRight;
      default:
         return anim;
   }
}
