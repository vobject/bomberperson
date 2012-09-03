#include "Player.hpp"
#include "Cell.hpp"
#include "Extra.hpp"
#include "Bomb.hpp"
#include "../input/InputDevice.hpp"
#include "../utils/Utils.hpp"

Player::Player(
   const EntityId player_id,
   const std::shared_ptr<InputDevice>& input
)
   : SceneObject(player_id)
   , mInput(input)
{

}

Player::~Player()
{

}

void Player::Update(const int elapsed_time)
{
   SetSound(SoundId::None);

   const auto old_state = mState;

   if (mParentCell->HasExplosion())
   {
      // Explosions kill the player instantly.
      SetSound(SoundId::PlayerDies);
      SetAlive(false);
      return;
   }

   if (mParentCell->HasExtra())
   {
      switch (mParentCell->CollectExtra()->GetId())
      {
         case EntityId::SpeedExtra:
            IncreaseSpeed();
            break;
         case EntityId::BombsExtra:
            mBombSupply++;
            break;
         case EntityId::RangeExtra:
            mBombRange++;
            break;
         case EntityId::GoldRangeExtra:
            mBombRange = 999;
            break;
         default:
            break;
      }
      SetSound(SoundId::PlayerPicksUpExtra);
   }

   UpdateMovement(elapsed_time);
   UpdateBombing(elapsed_time);

   if (old_state == mState) {
      mStateTime += elapsed_time;
   }
   else {
      mStateTime = 0; // Start new state.
   }
}

void Player::SetParentCell(const std::shared_ptr<Cell>& cell)
{
   mParentCell = cell;
}

PlayerState Player::GetState() const
{
   return mState;
}

int Player::GetStateTime() const
{
   return mStateTime;
}

int Player::GetSpeed() const
{
   // FIXME
   return mMovementSpeed;
}

void Player::UpdateMovement(const int elapsed_time)
{
   mMoveIdleTime += elapsed_time;
   if (mMoveIdleTime < mMovementSpeed) {
      return;
   }

   auto up = 0;
   auto down = 0;
   auto left = 0;
   auto right = 0;
   auto update_anim = false;

   if (mInput->TestUp())
   {
      update_anim = true;
      mState = PlayerState::WalkUp;

      if (CanMove(Direction::Up, mMovementDistance)) {
         up += mMovementDistance;
      }
   }
   if (mInput->TestDown())
   {
      update_anim = true;
      mState = PlayerState::WalkDown;

      if (CanMove(Direction::Down, mMovementDistance)) {
         down += mMovementDistance;
      }
   }
   if (mInput->TestLeft())
   {
      update_anim = true;
      mState = PlayerState::WalkLeft;

      if (CanMove(Direction::Left, mMovementDistance)) {
         left += mMovementDistance;
      }
   }
   if (mInput->TestRight())
   {
      update_anim = true;
      mState = PlayerState::WalkRight;

      if (CanMove(Direction::Right, mMovementDistance)) {
         right += mMovementDistance;
      }
   }

   SetPosition({ GetPosition().X - left + right, GetPosition().Y - up + down});

   if (!update_anim)
   {
      mState = GetStopWalkingState(mState);
   }

   mMoveIdleTime = 0;
}

void Player::UpdateBombing(const int elapsed_time)
{
   mBombIdleTime += elapsed_time;
   if (mBombIdleTime < mPlantingSpeed) {
      return;
   }

   if (!mInput->TestAction1() && !mInput->TestAction2())
   {
      // The user did not request to plant a bomb.
      return;
   }

   // TODO: Use Action2 input to detonate remote controlled bombs etc.

   if (mParentCell->HasBomb()) {
      // Only one bomb per cell.
      return;
   }

   if (!CanPlantBomb()) {
      // Out of bomb supply. Wait till an older bomb exploded.
      return;
   }

   auto bomb = std::make_shared<Bomb>(mParentCell);
   bomb->SetRange(mBombRange);
   bomb->SetSize(mParentCell->GetSize());
   bomb->SetPosition(mParentCell->GetPosition());
   bomb->SetSound(SoundId::BombPlanted);
   mParentCell->SetBomb(bomb);

   mPlantedBombs.push_back(bomb);
   mBombIdleTime = 0;
}

bool Player::CanMove(const Direction dir, const int distance) const
{
   const Point cell_pos = mParentCell->GetPosition();
   const Size cell_size = mParentCell->GetSize();

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
   const auto neighbor_cell = mParentCell->GetNeighborCell(dir);
   if (neighbor_cell &&
       !neighbor_cell->HasWall() &&
       !neighbor_cell->HasBomb())
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
      if (bomb->IsAlive())
      {
         bombs_alive++;
      }
   }

   // Garbage collection of the planted-bombs-vector.
   if (!bombs_alive) {
      mPlantedBombs.clear();
   }

   return (mBombSupply > bombs_alive);
}

void Player::IncreaseSpeed()
{
   if (mMovementSpeed > MAX_SPEED)
   {
      mMovementSpeed -= 2_ms;
   }
   else
   {
      mMovementDistance++;
   }
}

PlayerState Player::GetStopWalkingState(const PlayerState state) const
{
   switch (state)
   {
      case PlayerState::WalkUp:
         return PlayerState::StandUp;
      case PlayerState::WalkDown:
         return PlayerState::StandDown;
      case PlayerState::WalkLeft:
         return PlayerState::StandLeft;
      case PlayerState::WalkRight:
         return PlayerState::StandRight;
      default:
         return state;
   }
}
