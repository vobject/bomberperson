#include "Player.hpp"
#include "EntityManager.hpp"
#include "Arena.hpp"
#include "Extra.hpp"
#include "Bomb.hpp"
#include "Explosion.hpp"
#include "../utils/Utils.hpp"
#include "../Options.hpp"

Player::Player(
   const std::shared_ptr<Arena>& arena,
   const PlayerType type,
   EntityManager& entity_factory
)
   : ArenaObject(EntityId::Player, ZOrder::Layer_6, arena)
   , mType(type)
   , mData(PlayerAnimation::StandDown, MIN_SPEED, 1)
   , mSound(PlayerSound::None)
   , mEntityFactory(entity_factory)
{

}

Player::~Player()
{

}

void Player::Update(const int elapsed_time)
{
   if (PlayerAnimation::Dying == mData.anim)
   {
      // The player is currently dying. Nothing to do but wait.
      SetAnimationTime(GetAnimationTime() + elapsed_time);

      if (GetAnimationTime() >= DefaultValue::PLAYER_DEATH_ANIM_LEN)
      {
         // The death animation is over.
         Invalidate();
      }
      return;
   }

   const auto parent_cell = GetArena()->GetCellFromObject(*this);

   if (GetArena()->HasExplosion(parent_cell))
   {
      // Explosions kill the player. Prepare his death animation.
      SetAnimationTime(0);
      mData.anim = PlayerAnimation::Dying;
      mData.speed = 0;
      mSound = PlayerSound::Die;
      return;
   }

   // Original animation used to check if the player's animation
   //  changed during movement and other update methods.
   const auto old_anim = mData.anim;

   if (GetArena()->HasExtra(parent_cell))
   {
      switch (GetArena()->GetExtra(parent_cell)->GetType())
      {
         case ExtraType::Speed:
            IncreaseSpeed();
            mSound = PlayerSound::Collect_Speed;
            break;
         case ExtraType::Bombs:
            mData.bombs = std::min(mData.bombs + 1, 99);
            mSound = PlayerSound::Collect_Bombs;
            break;
         case ExtraType::Range:
            mData.range = std::min(mData.range + 1, 99);
            mSound = PlayerSound::Collect_Range;
            break;
         case ExtraType::InfiniteRange:
            mData.range = 99;
            mSound = PlayerSound::Collect_InfiniteRange;
            break;
         case ExtraType::RemoteBombs:
            mData.remote_bombs += 5;
            mSound = PlayerSound::Collect_Bombs;
            break;
         default:
            break;
      }
      // An extra should no longer exist after it was picked
      //  up by a player.
      GetArena()->DestroyExtra(parent_cell);
   }

   UpdateMovement(elapsed_time);
   UpdateBombing(elapsed_time);

   if (old_anim == mData.anim) {
      SetAnimationTime(GetAnimationTime() + elapsed_time);
   }
   else {
      SetAnimationTime(0); // Start new animation.
   }
}

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

PlayerSound Player::GetSound(const bool reset)
{
   const auto ret = mSound;

   if (reset) {
      mSound = PlayerSound::None;
   }
   return ret;
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

   const auto parent_cell = GetArena()->GetCellFromObject(*this);

   if (mCurrentCommands.action1 && // User pressed the right button.
       !GetArena()->HasBomb(parent_cell) && // No bomb in this cell yet.
       CanPlantBomb()) // Player did not run out of bomb supply.
   {
      auto bomb_type = BombType::Countdown; // The default bomb type.

      if (mData.remote_bombs)
      {
         // Always plant remote controlled bombs if the player has them.
         bomb_type = BombType::Remote;
         mData.remote_bombs--;
      }

      auto bomb = mEntityFactory.CreateBomb(parent_cell, bomb_type);
      bomb->SetRange(mData.range);
//      bomb->SetOwner(GetId());
//      bomb->SetSound(SoundId::BombPlanted);
      GetArena()->SetBomb(parent_cell, bomb);

      mPlantedBombs.push_back(bomb);
      mBombIdleTime = 0_ms;
   }

   if (mCurrentCommands.action2)
   {
      // Detonate all remote controlled bombs that the player planted.
      for (auto& bomb : mPlantedBombs)
      {
         if (bomb->IsValid() && (BombType::Remote == bomb->GetType()))
         {
            bomb->Detonate();
         }
      }
   }
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
      mData.speed -= 1_ms;
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
