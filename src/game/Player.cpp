#include "Player.hpp"
#include "Cell.hpp"
#include "Extra.hpp"
#include "Bomb.hpp"
#include "../input/InputDevice.hpp"
#include "../utils/Utils.hpp"

Player::Player(const std::string& res_name)
{
   SetResourceId(res_name);

//   mWalkUpAnimation.SetFrameCount(mWalkAnimationFrames);
//   mWalkUpAnimation.SetLength(mWalkAnimationLength);
//   mWalkUpAnimation.SetLooping(true);

//   mWalkDownAnimation.SetFrameCount(mWalkAnimationFrames);
//   mWalkDownAnimation.SetLength(mWalkAnimationLength);
//   mWalkDownAnimation.SetLooping(true);

//   mWalkLeftAnimation.SetFrameCount(mWalkAnimationFrames);
//   mWalkLeftAnimation.SetLength(mWalkAnimationLength);
//   mWalkLeftAnimation.SetLooping(true);

//   mWalkRightAnimation.SetFrameCount(mWalkAnimationFrames);
//   mWalkRightAnimation.SetLength(mWalkAnimationLength);
//   mWalkRightAnimation.SetLooping(true);
}

Player::~Player()
{

}

void Player::Update(const int elapsed_time)
{
   const auto old_state = mState;

   mMoveIdleTime += elapsed_time;
   if (mMoveIdleTime > mMovementSpeed)
   {
      UpdateMovement(mMoveIdleTime);
   }

   mBombIdleTime += elapsed_time;
   if (mBombIdleTime > mPlantingSpeed)
   {
      UpdateBombing(mBombIdleTime);
   }

   if (old_state == mState) {
      mStateTime += elapsed_time;
   }
   else {
      mStateTime = 0; // Start new state.
   }
}

std::shared_ptr<InputDevice> Player::GetInputDevice() const
{
   return mInput;
}

void Player::SetInputDevice(const std::shared_ptr<InputDevice>& input)
{
   mInput = input;
}

void Player::SetParentCell(const std::shared_ptr<Cell>& cell)
{
   mParentCell = cell;

   if (mParentCell->HasExplosion())
   {
      // Explosions kill the player instantly.
      SetAlive(false);
      return;
   }

   if (mParentCell->HasExtra())
   {
      switch (mParentCell->CollectExtra()->GetType())
      {
         case ExtraType::Speed:
            IncreaseSpeed();
            break;
         case ExtraType::BombRange:
            mBombRange++;
            break;
         case ExtraType::BombSupply:
            mBombSupply++;
            break;
         default:
            break;
      }
   }
}

//Direction Player::GetDirection() const
//{
//   return mDirection;
//}

//int Player::GetAnimationFrame() const
//{
//   return GetCurrentDirectionAnimation().GetCurrentFrame();
//}

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
   const auto distance = 1;
   auto up = 0;
   auto down = 0;
   auto left = 0;
   auto right = 0;
   auto update_anim = false;

   if (mInput->TestUp())
   {
      update_anim = true;
      mState = PlayerState::WalkUp;

      if (CanMove(Direction::Up, distance)) {
         up++;
      }
   }
   if (mInput->TestDown())
   {
      update_anim = true;
      mState = PlayerState::WalkDown;

      if (CanMove(Direction::Down, distance)) {
         down++;
      }
   }
   if (mInput->TestLeft())
   {
      update_anim = true;
      mState = PlayerState::WalkLeft;

      if (CanMove(Direction::Left, distance)) {
         left++;
      }
   }
   if (mInput->TestRight())
   {
      update_anim = true;
      mState = PlayerState::WalkRight;

      if (CanMove(Direction::Right, distance)) {
         right++;
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
   if (!mInput->TestPlantBomb())
   {
      // The user did not request to plant a bomb.
      return;
   }

   if (mParentCell->HasBomb()) {
      // Only one bomb per cell.
      return;
   }

   if (!CanPlantBomb()) {
      // Out of bomb supply. Wait till an older bomb exploded.
      return;
   }

   auto bomb = std::make_shared<Bomb>("bomb", mParentCell);
   bomb->SetRange(mBombRange);
   bomb->SetSize(mParentCell->GetSize());
   bomb->SetPosition(mParentCell->GetPosition());
   mParentCell->SetBomb(bomb);

   mPlantedBombs.push_back(bomb);
   mBombIdleTime = 0;
}

//void Player::UpdateAnimation(const int elapsed_time)
//{
//   switch (mDirection)
//   {
//      case Direction::Up:
//         mWalkUpAnimation.Update(elapsed_time);
//         mWalkDownAnimation.Reset();
//         mWalkLeftAnimation.Reset();
//         mWalkRightAnimation.Reset();
//         break;
//      case Direction::Down:
//         mWalkUpAnimation.Reset();
//         mWalkDownAnimation.Update(elapsed_time);
//         mWalkLeftAnimation.Reset();
//         mWalkRightAnimation.Reset();
//         break;
//      case Direction::Left:
//         mWalkUpAnimation.Reset();
//         mWalkDownAnimation.Reset();
//         mWalkLeftAnimation.Update(elapsed_time);
//         mWalkRightAnimation.Reset();
//         break;
//      case Direction::Right:
//         mWalkUpAnimation.Reset();
//         mWalkDownAnimation.Reset();
//         mWalkLeftAnimation.Reset();
//         mWalkRightAnimation.Update(elapsed_time);
//         break;
//   }
//}

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
   if (mMovementSpeed > 2) {
      mMovementSpeed -= 2;

//      mWalkAnimationLength -= 60;
//      mWalkUpAnimation.SetLength(mWalkAnimationLength);
//      mWalkDownAnimation.SetLength(mWalkAnimationLength);
//      mWalkLeftAnimation.SetLength(mWalkAnimationLength);
//      mWalkRightAnimation.SetLength(mWalkAnimationLength);
   }
   // The player is already at maximum speed.
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
