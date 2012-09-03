#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "SceneObject.hpp"
#include "Sound.hpp"
#include "../input/InputDevice.hpp"
#include "../utils/Utils.hpp"

#include <memory>
#include <vector>

class InputDevice;
class Cell;
class Bomb;

enum class PlayerSound
{

};

enum class PlayerState
{
   StandUp,
   StandDown,
   StandLeft,
   StandRight,
   WalkUp,
   WalkDown,
   WalkLeft,
   WalkRight,
//   Idle,
//   Lockedin,
//   Die,
//   Win
};

//struct PlayerData
//{
//   PlayerState state;
//   int state_time;

//   int speed;
//   int bombs;
//   int range;

//   bool reverse;

//   int wins;
//   int kills;
//};

//class PlayerAnimation
//{
//public:
//   PlayerAnimation() {}
//   ~PlayerAnimation() {}

//   PlayerState GetState() const { return mState; }
//   void SetState(PlayerState state) { mState = state; }

//   Direction GetDirection() const { return mDirection; }
//   void SetDirection(Direction dir) { mDirection = dir; }

//private:
//   PlayerState mState = PlayerState::Stand;
//   Direction mDirection = Direction::Down;
//};

class Player : public SceneObject
{
public:
   Player(const EntityId player_id, const std::shared_ptr<InputDevice>& input);
   virtual ~Player();

   Player(const Player&) = delete;
   Player& operator=(const Player&) = delete;

   void Update(int elapsed_time) override;

   void SetParentCell(const std::shared_ptr<Cell>& cell);

   PlayerState GetState() const;
   int GetStateTime() const;
   int GetSpeed() const;

private:
   static const int MIN_SPEED = 16_ms;
   static const int MAX_SPEED = 2_ms;

   void UpdateMovement(int elapsed_time);
   void UpdateBombing(int elapsed_time);
   void UpdateAnimation(int elapsed_time);

   bool CanMove(Direction dir, int distance) const;
   bool CanPlantBomb();

   void IncreaseSpeed();
   PlayerState GetStopWalkingState(PlayerState state) const;

   std::shared_ptr<InputDevice> mInput;
   std::shared_ptr<Cell> mParentCell;
   int mMoveIdleTime = 0;
   int mBombIdleTime = 0;

   // Number of milliseconds the player has to wait to move another pixel.
   int mMovementSpeed = MIN_SPEED;
   int mMovementDistance = 1;

   // How many milliseconds does the player have to wait to plant another bomb?
   int mPlantingSpeed = 200_ms;

   int mBombRange = 1;
   int mBombSupply = 1;
   std::vector<std::shared_ptr<Bomb>> mPlantedBombs;

   PlayerState mState = PlayerState::StandDown;
   int mStateTime = 0;
};

#endif // PLAYER_HPP
