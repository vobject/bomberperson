#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "SceneObject.hpp"
#include "Animation.hpp"
#include "../input/InputDevice.hpp"
#include "../utils/Utils.hpp"

#include <memory>
#include <vector>

class InputDevice;
class Cell;
class Bomb;

class Player : public SceneObject
{
public:
   Player(const std::string& res_name, const std::shared_ptr<InputDevice>& input);
   virtual ~Player();

   Player(const Player&) = delete;
   Player& operator=(const Player&) = delete;

   void Update(int elapsed_time) override;

//   std::shared_ptr<InputDevice> GetInputDevice() const;
//   void SetInputDevice(const std::shared_ptr<InputDevice>& input);

   void SetParentCell(const std::shared_ptr<Cell>& cell);

   Direction GetDirection() const;
   int GetAnimationFrame() const;

private:
   void UpdateMovement(int elapsed_time);
   void UpdateBombing(int elapsed_time);
   void UpdateAnimation(int elapsed_time);

   bool CanMove(Direction dir, int distance) const;
   bool CanPlantBomb();

   void IncreaseSpeed();

   const Animation& GetCurrentDirectionAnimation() const;

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

   Direction mDirection = Direction::Down;
   Animation mWalkUpAnimation;
   Animation mWalkDownAnimation;
   Animation mWalkLeftAnimation;
   Animation mWalkRightAnimation;
//   Animation mDeathAnimation;
//   Animation mWinAnimation;
};

#endif // PLAYER_HPP
