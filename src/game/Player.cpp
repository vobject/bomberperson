#include "Player.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"
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
   , mEventQueue(queue)
{
   mEventQueue.Register(this);

   mAnimation = PlayerAnimation::Spawn;
   mSound = PlayerSound::Spawn;
   mSpeed = 0; // Might be used for animation speed.
}

Player::~Player()
{
   mEventQueue.UnRegister(this);
}

void Player::Update(const int elapsed_time)
{
   SetAnimationTime(GetAnimationTime() + elapsed_time);

   if (PlayerAnimation::Spawn == mAnimation)
   {
      if (GetAnimationTime() >= DefaultValue::PLAYER_SPAWN_ANIM_LEN)
      {
         // Set the players default values, now that he spawned.
         mAnimation = PlayerAnimation::StandDown;
         mSound = PlayerSound::None;
         mSpeed = MIN_SPEED;
         SetAnimationTime(0);
      }
      return;
   }

   if (PlayerAnimation::Destroy == mAnimation)
   {
      if (GetAnimationTime() >= DefaultValue::PLAYER_DEATH_ANIM_LEN)
      {
         // The death animation is over. Remove the player from the game.
         mEventQueue.Add(std::make_shared<RemovePlayerEvent>(GetInstanceId(),
                                                             GetType()));
      }
      return;
   }

   const auto arena = GetArena();
   const auto parent_cell = arena->GetCellFromObject(*this);

   if (arena->HasExplosion(parent_cell))
   {
      // Explosions kill the player. Prepare his death animation.
      const auto killer = arena->GetExplosionOwner(parent_cell);
      mEventQueue.Add(std::make_shared<KillPlayerEvent>(GetInstanceId(),
                                                        GetType(),
                                                        killer));
      return;
   }

   if (arena->HasExtra(parent_cell)) {
      PickupExtra(parent_cell);
   }

   UpdateMovement(elapsed_time);
   UpdateBombing(elapsed_time);
}

void Player::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::RemovePlayer:
         OnRemovePlayer(dynamic_cast<const RemovePlayerEvent&>(event));
         break;
      case EventType::Input:
         OnInput(dynamic_cast<const InputEvent&>(event));
         break;
      case EventType::RemoveBomb:
         OnRemoveBomb(dynamic_cast<const RemoveBombEvent&>(event));
         break;
      case EventType::KillPlayer:
         OnKillPlayer(dynamic_cast<const KillPlayerEvent&>(event));
         break;
      default:
         break;
   }
}

PlayerType Player::GetType() const
{
   return mType;
}

PlayerAnimation Player::GetAnimation() const
{
   return mAnimation;
}

int Player::GetSpeed() const
{
   return mSpeed;
}

int Player::GetDistance() const
{
   return mDistance;
}

PlayerSound Player::GetSound(const bool reset)
{
   const auto ret = mSound;

   if (reset) {
      mSound = PlayerSound::None;
   }
   return ret;
}

void Player::OnRemovePlayer(const RemovePlayerEvent& event)
{
   if (event.GetPlayer() != GetType()) {
      return;
   }

   Invalidate();
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

void Player::OnRemoveBomb(const RemoveBombEvent& event)
{
   if (event.GetOwner() != GetType()) {
      return;
   }

   // This is a bomb we planted some time ago.
   // It did explosde and does not count as 'planted' anymore.
   mBombsPlanted--;
}

void Player::OnKillPlayer(const KillPlayerEvent& event)
{
   if (event.GetVictim() != GetType())
   {
      // We were not the victim of this kill-event.
      // Check if we actually killed another player.
      if (event.GetKiller() == GetType())
      {
         // Keep track of the player we killed.
         mKills.push_back(event.GetVictim());
      }
      return;
   }

   // We were killed.
   SetAnimationTime(0);
   mAnimation = PlayerAnimation::Destroy;
   mSound = PlayerSound::Destroy;
   mSpeed = 0; // Might be used for animation speed.
}

void Player::PickupExtra(const Cell cell)
{
   switch (GetArena()->GetExtraType(cell))
   {
      case ExtraType::Speed:
         IncreaseSpeed();
         mSound = PlayerSound::Collect_Speed;
         break;
      case ExtraType::Bombs:
         mBombSupply = std::min(mBombSupply + 1, 99);
         mSound = PlayerSound::Collect_Bombs;
         break;
      case ExtraType::Range:
         mBombRange = std::min(mBombRange + 1, 99);
         mSound = PlayerSound::Collect_Range;
         break;
      case ExtraType::InfiniteRange:
         mBombRange = 99;
         mSound = PlayerSound::Collect_InfiniteRange;
         break;
      case ExtraType::Kick:
         mCanKick = true;
         mSound = PlayerSound::Collect_Speed;
         break;
      case ExtraType::RemoteBombs:
         mRemoteBombs += 5;
         mSound = PlayerSound::Collect_Bombs;
         break;
      default:
         break;
   }

   // An extra should no longer exist after it was picked up by a player.
   mEventQueue.Add(std::make_shared<RemoveExtraEvent>(GetInstanceId(),
                                                      cell));
}

void Player::UpdateMovement(const int elapsed_time)
{
   mMoveIdleTime += elapsed_time;
   if (mMoveIdleTime < mSpeed) {
      return;
   }

   auto up = 0;
   auto down = 0;
   auto left = 0;
   auto right = 0;
   auto got_input = false;

   if (mInputUp)
   {
      got_input = true;
      mAnimation = PlayerAnimation::WalkUp;

      if (CanMove(Direction::Up, mDistance)) {
         up += mDistance;
      }
   }

   if (mInputDown)
   {
      got_input = true;
      mAnimation = PlayerAnimation::WalkDown;

      if (CanMove(Direction::Down, mDistance)) {
         down += mDistance;
      }
   }

   if (mInputLeft)
   {
      got_input = true;
      mAnimation = PlayerAnimation::WalkLeft;

      if (CanMove(Direction::Left, mDistance)) {
         left += mDistance;
      }
   }

   if (mInputRight)
   {
      got_input = true;
      mAnimation = PlayerAnimation::WalkRight;

      if (CanMove(Direction::Right, mDistance)) {
         right += mDistance;
      }
   }

   SetPosition({ GetPosition().X - left + right, GetPosition().Y - up + down});

   if (!got_input)
   {
      // Select the right stand-still animation in case we did not move.
      mAnimation = GetStopWalkingState(mAnimation);
   }

   // Reset the movement timer if there was input for the player, so
   //  that we have to wait for the idle time cycle until we can move again.
   mMoveIdleTime = 0_ms;
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
       (mBombSupply > mBombsPlanted)) // Player did not run out of bomb supply.
   {
      auto bomb_type = BombType::Countdown; // The default bomb type.

      if (mRemoteBombs)
      {
         // Always plant remote controlled bombs if the player has them.
         bomb_type = BombType::Remote;
         mRemoteBombs--;
      }

      mBombsPlanted++;
      mBombIdleTime = 0_ms;

      mEventQueue.Add(std::make_shared<CreateBombEvent>(GetInstanceId(),
                                                        parent_cell,
                                                        bomb_type,
                                                        GetType(),
                                                        mBombRange));
   }

   if (mInputAction2)
   {
      // Detonate all remote controlled bombs that the player planted.
      mEventQueue.Add(std::make_shared<DetonateRemoteBombEvent>(GetInstanceId(),
                                                                GetType()));
      mBombIdleTime = 0_ms;
   }
}

bool Player::CanMove(const Direction dir, const int distance) const
{
   const auto arena = GetArena();
   const auto parent_cell = arena->GetCellFromObject(*this);
   const auto cell_size = arena->GetCellSize();
   const auto cell_pos = arena->GetCellPosition(parent_cell);

   // Check for movement inside the current cell.
   // Movement inside the current cell is always ok.
   switch (dir)
   {
      case Direction::Up:
         if ((GetPosition().Y - distance) >= cell_pos.Y) {
            return true;
         }
         break;
      case Direction::Down:
         if ((GetPosition().Y + GetSize().Height + distance) <= (cell_pos.Y + cell_size.Height)) {
            return true;
         }
         break;
      case Direction::Left:
         if ((GetPosition().X - distance) >= cell_pos.X) {
            return true;
         }
         break;
      case Direction::Right:
         if ((GetPosition().X + GetSize().Width + distance) <= (cell_pos.X + cell_size.Width)) {
            return true;
         }
         break;
   }

   // Player wants to walk to another cell - check if that is allowed.
   const auto neighbor_cell = arena->GetNeighborCell(parent_cell, dir);

   if ((-1 == neighbor_cell.X) || (-1 == neighbor_cell.Y)) {
      // There is no cell in this direction.
      return false;
   }

   if (arena->HasWall(neighbor_cell)) {
      // Players cannot walk through walls.
      return false;
   }

   if (arena->HasBomb(neighbor_cell) && !mCanKick) {
       // A bomb blocks the path and the player does not have the kick-extra.
      return false;
   }

   if (arena->HasBomb(neighbor_cell) && mCanKick)
   {
      // A bomb blocks the player path but we can try to kick it.
      // In any way, return false, because the bomb first has to move.
      mEventQueue.Add(std::make_shared<MoveBombEvent>(GetInstanceId(),
                                                      neighbor_cell,
                                                      mSpeed,
                                                      mDistance,
                                                      dir));
      return false;
   }

   // No Wall, no bomb - movement is fine.
   return true;
}

void Player::IncreaseSpeed()
{
   if (mSpeed > MAX_SPEED)
   {
      mSpeed -= 1_ms;
   }
   else
   {
      // The player will now move an additional pixel per interval
      //  if he really got this fast.
      mDistance++;
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
