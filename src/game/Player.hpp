#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "SceneObject.hpp"
//#include "Animation.hpp"
#include "../input/InputDevice.hpp"
#include "../utils/Utils.hpp"

#include <memory>
#include <vector>

class InputDevice;
class Cell;
class Bomb;

//enum class PlayerState
//{
//   Stand,
//   Walk,
////   Idle,
////   Lockedin,
////   Die,
////   Win
//};

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
   Player(const std::string& res_name);
   virtual ~Player();

   Player(const Player&) = delete;
   Player& operator=(const Player&) = delete;

   void Update(int elapsed_time) override;

   std::shared_ptr<InputDevice> GetInputDevice() const;
   void SetInputDevice(const std::shared_ptr<InputDevice>& input);

   void SetParentCell(const std::shared_ptr<Cell>& cell);

   Direction GetDirection() const;
   int GetAnimationFrame() const;

   PlayerState GetState() const;
   int GetStateTime() const;
   int GetSpeed() const;

private:
   static const int SPEED_MIN = 1;
   static const int SPEED_MAX = 20;

   void UpdateMovement(int elapsed_time);
   void UpdateBombing(int elapsed_time);
   void UpdateAnimation(int elapsed_time);

   bool CanMove(Direction dir, int distance) const;
   bool CanPlantBomb();

   void IncreaseSpeed();
   PlayerState GetStopWalkingState(PlayerState state) const;

//   const Animation& GetCurrentDirectionAnimation() const;

   std::shared_ptr<InputDevice> mInput;
   std::shared_ptr<Cell> mParentCell;
   int mMoveIdleTime = 0;
   int mBombIdleTime = 0;

   // Number of milliseconds the player has to wait to move another pixel.
   int mMovementSpeed = 8;
   // How many milliseconds does the player have to wait to plant another bomb?
   int mPlantingSpeed = 200;

   int mWalkAnimationFrames = 2;
   int mWalkAnimationLength = mMovementSpeed * 40;

   int mBombRange = 1;
   int mBombSupply = 1;
   std::vector<std::shared_ptr<Bomb>> mPlantedBombs;

//   Direction mDirection = Direction::Down;
//   Animation mWalkUpAnimation;
//   Animation mWalkDownAnimation;
//   Animation mWalkLeftAnimation;
//   Animation mWalkRightAnimation;
//   Animation mDeathAnimation;
//   Animation mWinAnimation;

   PlayerState mState = PlayerState::StandDown;
   int mStateTime = 0;
};

#endif // PLAYER_HPP
