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
         // The spawning animation is over.
         mEventQueue.Add(std::make_shared<SpawnPlayerEndEvent>(GetInstanceId()));
      }
      // The player is currently spawning. Nothing to do but wait.
      return;
   }

   if (PlayerAnimation::Destroy == mAnimation)
   {
      // The player is currently dying. Nothing to do but wait.
      if (GetAnimationTime() >= DefaultValue::PLAYER_DEATH_ANIM_LEN)
      {
         // The death animation is over.
         mEventQueue.Add(std::make_shared<DestroyPlayerEndEvent>(GetInstanceId()));
      }
      return;
   }

   const auto arena = GetArena();
   const auto parent_cell = arena->GetCellFromObject(*this);

   if (arena->HasExtra(parent_cell))
   {
      mEventQueue.Add(std::make_shared<PickupExtraEvent>(GetInstanceId(),
                                                         parent_cell));
   }

   UpdateMovement(elapsed_time);
   UpdateBombing(elapsed_time);
}

void Player::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::SpawnPlayerStart:
         OnSpawnPlayerStart(dynamic_cast<const SpawnPlayerStartEvent&>(event));
         break;
      case EventType::SpawnPlayerEnd:
         OnSpawnPlayerEnd(dynamic_cast<const SpawnPlayerEndEvent&>(event));
         break;
      case EventType::DestroyPlayerStart:
         OnDestroyPlayerStart(dynamic_cast<const DestroyPlayerStartEvent&>(event));
         break;
      case EventType::DestroyPlayerEnd:
         OnDestroyPlayerEnd(dynamic_cast<const DestroyPlayerEndEvent&>(event));
         break;
      case EventType::SpawnBombStart:
         OnSpawnBombStart(dynamic_cast<const SpawnBombStartEvent&>(event));
         break;
      case EventType::DestroyBombEnd:
         OnDestroyBombEnd(dynamic_cast<const DestroyBombEndEvent&>(event));
         break;
      case EventType::SpawnExplosionStart:
         OnSpawnExplosionStart(dynamic_cast<const SpawnExplosionStartEvent&>(event));
         break;
      case EventType::Input:
         OnInput(dynamic_cast<const InputEvent&>(event));
         break;
      case EventType::MovePlayer:
         OnMovePlayer(dynamic_cast<const MovePlayerEvent&>(event));
         break;
      case EventType::PickupExtra:
         OnPickupExtra(dynamic_cast<const PickupExtraEvent&>(event));
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

void Player::OnSpawnPlayerStart(const SpawnPlayerStartEvent& event)
{
   if (event.GetPlayer() != GetType()) {
      // This event is not for us.
      return;
   }

   SetAnimationTime(0);
   mAnimation = PlayerAnimation::Spawn;
   mSound = PlayerSound::Spawn;
   mSpeed = 0; // Might be used for animation speed.
   SetVisible(true);
}

void Player::OnSpawnPlayerEnd(const SpawnPlayerEndEvent& event)
{
   if (event.GetSender() != GetInstanceId()) {
      // This event is not for us.
      return;
   }

   // Set the players default values, now that the spawning animation is over.
   mAnimation = PlayerAnimation::StandDown;
   mSound = PlayerSound::None;
   mSpeed = MIN_SPEED;
   SetAnimationTime(0);
}

void Player::OnDestroyPlayerStart(const DestroyPlayerStartEvent& event)
{
   if (event.GetPlayer() != GetType())
   {
      // We were not the victim of this kill-event.
      // Check if we actually killed another player.
      if (event.GetKiller() == GetType())
      {
         // Keep track of the player we killed.
         mKills.push_back(event.GetPlayer());
      }
      return;
   }

   // We were killed.
   SetAnimationTime(0);
   mAnimation = PlayerAnimation::Destroy;
   mSound = PlayerSound::Destroy;
   mSpeed = 0; // Might be used for animation speed.
}

void Player::OnDestroyPlayerEnd(const DestroyPlayerEndEvent& event)
{
   if (event.GetSender() != GetInstanceId()) {
      // This event is not for us.
      return;
   }

   SetVisible(false);
   Invalidate();

   // The death animation is over. Remove the player from the game.
   mEventQueue.Add(std::make_shared<RemovePlayerEvent>(GetInstanceId()));
}

void Player::OnSpawnBombStart(const SpawnBombStartEvent& event)
{
   if (event.GetOwner() != GetType()) {
      // This event is not for us.
      return;
   }

   if (BombType::Remote == event.GetBombType()) {
      mRemoteBombs--;
   }

   mBombsPlanted++;
   mBombIdleTime = 0_ms;
}

void Player::OnDestroyBombEnd(const DestroyBombEndEvent& event)
{
   if (event.GetOwner() != GetType()) {
      // This event is not for us.
      return;
   }

   // This is the center explosion of a bomb we planted some time ago.
   // It did explosde and does not cound as 'planted' anymore.
   mBombsPlanted--;
}

void Player::OnSpawnExplosionStart(const SpawnExplosionStartEvent& event)
{
   const auto parent_cell = GetArena()->GetCellFromObject(*this);

   if (parent_cell != event.GetCell()) {
      // The explosion does not affect us.
      return;
   }

   // Explosions kill the player. Prepare his death animation.
   mEventQueue.Add(std::make_shared<DestroyPlayerStartEvent>(GetInstanceId(),
                                                             GetType(),
                                                             event.GetOwner()));
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
   if (event.GetSender() != GetInstanceId()) {
      // This event is not for us.
      return;
   }

   auto up = 0;
   auto down = 0;
   auto left = 0;
   auto right = 0;
   const auto movement_data = event.GetMovementData();
   const auto old_anim = mAnimation;

   for (const auto& dir_dist : movement_data)
   {
      switch (dir_dist.first)
      {
         case Direction::Up:
            mAnimation = PlayerAnimation::WalkUp;
            up += dir_dist.second;
            break;
         case Direction::Down:
            mAnimation = PlayerAnimation::WalkDown;
            down += dir_dist.second;
            break;
         case Direction::Left:
            mAnimation = PlayerAnimation::WalkLeft;
            left += dir_dist.second;
            break;
         case Direction::Right:
            mAnimation = PlayerAnimation::WalkRight;
            right += dir_dist.second;
            break;
      }

      SetPosition({ GetPosition().X - left + right, GetPosition().Y - up + down});
   }

   if (movement_data.empty())
   {
      // Select the right stand-still animation in case we did not move.
      mAnimation = GetStopWalkingState(mAnimation);
   }
   else
   {
      // Reset the movement timer if there was input for the player, so
      //  that we have to wait for the idle time cycle until we can move again.
      mMoveIdleTime = 0_ms;
   }

   if (old_anim != mAnimation) {
      SetAnimationTime(0); // Start a new animation.
   }
}

void Player::OnPickupExtra(const PickupExtraEvent& event)
{
   if (event.GetSender() != GetInstanceId()) {
      // This event is not for us.
      return;
   }

   if (!GetArena()->HasExtra(event.GetCell())) {
      // The extra has already disappeared.
      return;
   }

   auto extra = GetArena()->GetExtra(event.GetCell());

   switch (extra->GetType())
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
   extra->Invalidate();
}

void Player::UpdateMovement(const int elapsed_time)
{
   mMoveIdleTime += elapsed_time;
   if (mMoveIdleTime < mSpeed) {
      return;
   }

   std::vector<std::pair<Direction, int>> dir_dist;

   if (mInputUp)
   {
      auto distance = 0;
      if (CanMove(Direction::Up, mDistance)) {
         distance = mDistance;
      }
      dir_dist.push_back({ Direction::Up, distance });
   }

   if (mInputDown)
   {
      auto distance = 0;
      if (CanMove(Direction::Down, mDistance)) {
         distance = mDistance;
      }
      dir_dist.push_back({ Direction::Down, distance });
   }

   if (mInputLeft)
   {
      auto distance = 0;
      if (CanMove(Direction::Left, mDistance)) {
         distance = mDistance;
      }
      dir_dist.push_back({ Direction::Left, distance });
   }

   if (mInputRight)
   {
      auto distance = 0;
      if (CanMove(Direction::Right, mDistance)) {
         distance = mDistance;
      }
      dir_dist.push_back({ Direction::Right, distance });
   }

   if (!dir_dist.empty() ||
         ((mAnimation != PlayerAnimation::StandUp) &&
          (mAnimation != PlayerAnimation::StandDown) &&
          (mAnimation != PlayerAnimation::StandLeft) &&
          (mAnimation != PlayerAnimation::StandRight)))
   {
      // Create an event in case:
      //  1. The player requested to move.
      //  2. The player was moving before (results in a stand-still event).
      mEventQueue.Add(std::make_shared<MovePlayerEvent>(GetInstanceId(),
                                                        dir_dist));
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
       (mBombSupply > mBombsPlanted)) // Player did not run out of bomb supply.
   {
      auto bomb_type = BombType::Countdown; // The default bomb type.

      if (mRemoteBombs)
      {
         // Always plant remote controlled bombs if the player has them.
         bomb_type = BombType::Remote;
      }

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
      const auto bomb = arena->GetBombInstanceId(neighbor_cell);
      mEventQueue.Add(std::make_shared<MoveBombEvent>(GetInstanceId(),
                                                      bomb,
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
      mSpeed -= 2_ms;
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
