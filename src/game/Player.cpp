#include "Player.hpp"
#include "EventQueue.hpp"
#include "Arena.hpp"
#include "Extra.hpp"
#include "Bomb.hpp"
#include "Explosion.hpp"
#include "../utils/Utils.hpp"
#include "../Options.hpp"

Player::Player(
   const std::shared_ptr<Arena>& arena,
   const PlayerType type,
   EventQueue& queue
)
   : ArenaObject(EntityId::Player, ZOrder::Layer_6, arena)
   , mType(type)
   , mData(PlayerAnimation::Spawn, MIN_SPEED, 1)
   , mSound(PlayerSound::None)
   , mEventQueue(queue)
{
   mEventQueue.Register(this);
}

Player::~Player()
{
   mEventQueue.UnRegister(this);
}

void Player::Update(const int elapsed_time)
{
   if (PlayerAnimation::Spawn == mData.anim)
   {
      // The player is currently spawning. Nothing to do but wait.
      SetAnimationTime(GetAnimationTime() + elapsed_time);

      if (GetAnimationTime() >= DefaultValue::PLAYER_SPAWN_ANIM_LEN)
      {
         // The spawning animation is over.
         mData.anim = PlayerAnimation::StandDown;
      }
      return;
   }

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
      const auto explosion_owner = GetArena()->GetExplosion(parent_cell)->GetOwner();
//      if (explosion_owner.get() != this) {
//         // We did not commit suicide. Increase kill counter for the owner.
//         explosion_owner->IncrementKills(GetType());
//      }

      // Explosions kill the player. Prepare his death animation.
      SetAnimationTime(0);
      mData.anim = PlayerAnimation::Dying;
      mData.speed = 0;
      mSound = PlayerSound::Die;
      return;
   }

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
         case ExtraType::Kick:
            mData.can_kick = true;
            mSound = PlayerSound::Collect_Speed;
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

   SetAnimationTime(GetAnimationTime() + elapsed_time);
}

void Player::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::CreateBomb:
         OnCreateBomb(dynamic_cast<const CreateBombEvent&>(event));
         break;
      case EventType::CreateExplosion:
         OnCreateExplosion(dynamic_cast<const CreateExplosionEvent&>(event));
         break;
      case EventType::Input:
         OnInput(dynamic_cast<const InputEvent&>(event));
         break;
      case EventType::MovePlayer:
         OnMovePlayer(dynamic_cast<const MovePlayerEvent&>(event));
         break;
      default:
         break;
   }
}

void Player::OnCreateBomb(const CreateBombEvent& event)
{
   if (event.GetOwner() != GetType()) {
      // This event is not for us.
      return;
   }

   if (BombType::Remote == event.GetBombType()) {
      mData.remote_bombs--;
   }

   mBombsPlanted++;
   mBombIdleTime = 0_ms;
}

void Player::OnCreateExplosion(const CreateExplosionEvent& event)
{
   if (event.GetOwner() != GetType()) {
      // This event is not for us.
      return;
   }

   if (event.GetExplosionType() != ExplosionType::Center) {
      // Not the explosion type we are looking for.
      return;
   }

   // This is the center explosion of a bomb we planted some time ago.
   // It did explosde and does not cound as 'planted' anymore.
   mBombsPlanted--;
}

void Player::OnInput(const InputEvent& event)
{
   if (event.GetPlayerType() != GetType()) {
      // This event is not for us.
      return;
   }

   mInputUp = event.GetUp();
   mInputDown = event.GetDown();
   mInputLeft = event.GetLeft();
   mInputRight = event.GetRight();
   mInputAction1 = event.GetAction1();
   mInputAction2 = event.GetAction2();
}

void Player::OnMovePlayer(const MovePlayerEvent& event)
{
   if (event.GetPlayerType() != GetType()) {
      // This event is not for us.
      return;
   }

   // The original animation - used to check if the player's
   //  animation changed during movement.
   const auto old_anim = mData.anim;

   auto up = 0;
   auto down = 0;
   auto left = 0;
   auto right = 0;
   const auto movement_data = event.GetMovementData();

   for (const auto& dir_dist : movement_data)
   {
      switch (dir_dist.first)
      {
         case Direction::Up:
            mData.anim = PlayerAnimation::WalkUp;
            up += dir_dist.second;
            break;
         case Direction::Down:
            mData.anim = PlayerAnimation::WalkDown;
            down += dir_dist.second;
            break;
         case Direction::Left:
            mData.anim = PlayerAnimation::WalkLeft;
            left += dir_dist.second;
            break;
         case Direction::Right:
            mData.anim = PlayerAnimation::WalkRight;
            right += dir_dist.second;
            break;
      }

      SetPosition({ GetPosition().X - left + right, GetPosition().Y - up + down});
   }

   if (movement_data.empty())
   {
      // Select the right stand-still animation in case we did not move.
      mData.anim = GetStopWalkingState(mData.anim);
   }
   else
   {
      // Reset the movement timer if there was input for the player, so
      //  that we have to wait for the idle time cycle until we can move again.
      mMoveIdleTime = 0_ms;
   }

   if (old_anim != mData.anim) {
      SetAnimationTime(0); // Start new animation.
   }
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

//void Player::IncrementKills(const PlayerType type)
//{
//   // Ignore which player was killed by now.
//   (void) type;

//   mData.kills++;
//}

void Player::UpdateMovement(const int elapsed_time)
{
   mMoveIdleTime += elapsed_time;
   if (mMoveIdleTime < mData.speed) {
      return;
   }

   std::vector<std::pair<Direction, int>> dir_dist;

   if (mInputUp)
   {
      auto distance = 0;
      if (CanMove(Direction::Up, mData.distance)) {
         distance = mData.distance;
      }
      dir_dist.push_back({ Direction::Up, distance });
   }

   if (mInputDown)
   {
      auto distance = 0;
      if (CanMove(Direction::Down, mData.distance)) {
         distance = mData.distance;
      }
      dir_dist.push_back({ Direction::Down, distance });
   }

   if (mInputLeft)
   {
      auto distance = 0;
      if (CanMove(Direction::Left, mData.distance)) {
         distance = mData.distance;
      }
      dir_dist.push_back({ Direction::Left, distance });
   }

   if (mInputRight)
   {
      auto distance = 0;
      if (CanMove(Direction::Right, mData.distance)) {
         distance = mData.distance;
      }
      dir_dist.push_back({ Direction::Right, distance });
   }

   if (!dir_dist.empty() ||
         ((mData.anim != PlayerAnimation::StandUp) &&
          (mData.anim != PlayerAnimation::StandDown) &&
          (mData.anim != PlayerAnimation::StandLeft) &&
          (mData.anim != PlayerAnimation::StandRight)))
   {
      // Create an event in case:
      //  1. The player requested to move.
      //  2. The player was moving before (results in a stand-still event).
      mEventQueue.Add(std::make_shared<MovePlayerEvent>(GetType(), dir_dist));
   }
}

void Player::UpdateBombing(const int elapsed_time)
{
   mBombIdleTime += elapsed_time;
   if (mBombIdleTime < mPlantingSpeed) {
      return;
   }

   const auto parent_cell = GetArena()->GetCellFromObject(*this);

   if (mInputAction1 && // User pressed the right button.
       !GetArena()->HasBomb(parent_cell) && // No bomb in this cell yet.
       (mData.bombs > mBombsPlanted)) // Player did not run out of bomb supply.
   {
      auto bomb_type = BombType::Countdown; // The default bomb type.

      if (mData.remote_bombs)
      {
         // Always plant remote controlled bombs if the player has them.
         bomb_type = BombType::Remote;
      }

      mEventQueue.Add(std::make_shared<CreateBombEvent>(parent_cell,
                                                        bomb_type,
                                                        mData.range,
                                                        GetType()));
   }

   if (mInputAction2)
   {
      // Detonate all remote controlled bombs that the player planted.
      mEventQueue.Add(std::make_shared<DetonateRemoteBombEvent>(GetType()));
   }
}

bool Player::CanMove(const Direction dir, const int distance) const
{
//   mParentCellChanged = false;
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

   // Player wants to walk to another cell - check if that is allowed.
   const auto neighbor_cell = arena->GetNeighborCell(parent_cell, dir);
   if ((-1 != neighbor_cell.X) &&
       (-1 != neighbor_cell.Y) &&
       !arena->HasWall(neighbor_cell))
   {
      // We may probably move into the next cell. There is one last thing
      //  we need to check: does it have a (movable) bomb?
      if (!arena->HasBomb(neighbor_cell)
//          ||
//          (mData.can_kick && arena->GetBomb(neighbor_cell)->CanMove(dir, distance))
          )
      {
//         mParentCellChanged = true;

         // A cell exists and does not block the player.
         return true;
      }
   }
   return false;
}

bool Player::CanPlantBomb()
{
   int bombs_alive = 0;
//   for (const auto& bomb : mPlantedBombs)
//   {
//      if (bomb->IsValid())
//      {
//         bombs_alive++;
//      }
//   }

//   // Garbage collection of the planted-bombs-vector.
//   if (!bombs_alive) {
//      mPlantedBombs.clear();
//   }

   return (mData.bombs > bombs_alive);
}

//void Player::KickBomb(const Direction dir) const
//{
//   if (!mParentCellChanged || !mData.can_kick) {
//      return;
//   }

//   const auto parent_cell = GetArena()->GetCellFromObject(*this);
//   const auto neighbor_cell = GetArena()->GetNeighborCell(parent_cell, dir);

//   if ((-1 != neighbor_cell.X) &&
//       (-1 != neighbor_cell.Y) &&
//       GetArena()->HasBomb(neighbor_cell))
//   {
//      GetArena()->GetBomb(neighbor_cell)->Move(dir, mData.speed, mData.distance);
//   }
//}

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

Direction Player::GetPlayerDirection() const
{
   switch (mData.anim)
   {
      case PlayerAnimation::WalkUp:
      case PlayerAnimation::StandUp:
         return Direction::Up;
      case PlayerAnimation::WalkDown:
      case PlayerAnimation::StandDown:
         return Direction::Down;
      case PlayerAnimation::WalkLeft:
      case PlayerAnimation::StandLeft:
         return Direction::Left;
      case PlayerAnimation::WalkRight:
      case PlayerAnimation::StandRight:
         return Direction::Right;
      default:
         break;
   }
   throw "Player: Invalid player direction.";
}
