#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "SceneObject.hpp"
#include "../input/InputDevice.hpp"
#include "../utils/Utils.hpp"

#include <memory>
#include <vector>

class EntityManager;
class Cell;
class Bomb;

// TODO: Should belong to some sort of RenderInfo data structure.
//  Because it is not really useful for the game logic.
enum class PlayerAnimation
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

struct PlayerData
{
   constexpr PlayerData(const PlayerAnimation anim,
                        const int anim_time,
                        const int speed,
                        const int distance,
                        const int bombs,
                        const int range,
                        const int wins,
                        const int kills)
      : anim(PlayerAnimation::StandDown)
      , anim_time(0_ms)
      , speed(speed)
      , distance(distance)
      , bombs(bombs)
      , range(range)
      , wins(wins)
      , kills(kills)
   { }

   PlayerAnimation anim;
   int anim_time;

   int speed;
   int distance;
   int bombs;
   int range;

   int wins;
   int kills;
};

class Player : public SceneObject
{
public:
   Player(EntityId player_id, EntityManager& entity_factory);
   virtual ~Player();

   Player(const Player&) = delete;
   Player& operator=(const Player&) = delete;

   void Update(int elapsed_time) override;

   void SetParentCell(const std::shared_ptr<Cell>& cell);
   void SetInputCommands(InputCommands cmds);

   PlayerData GetData() const;

private:
   // Number of milliseconds the player has to wait to move another pixel.
   static const int MIN_SPEED = 16_ms;
   static const int MAX_SPEED = 2_ms;

   // Number of milliseconds the player has to wait to plant another bomb.
   static const int MIN_PLANTING_SPEED = 250_ms;

   void UpdateMovement(int elapsed_time);
   void UpdateBombing(int elapsed_time);
   void UpdateAnimation(int elapsed_time);

   bool CanMove(Direction dir, int distance) const;
   bool CanPlantBomb();

   void IncreaseSpeed();
   PlayerAnimation GetStopWalkingState(PlayerAnimation state) const;

   EntityManager& mEntityFactory;

   std::shared_ptr<Cell> mParentCell;
   InputCommands mCurrentCommands;

   int mMoveIdleTime = 0_ms;
   int mBombIdleTime = 0_ms;

   // Move this into the PlayerData object if we choose to make
   //  it alterable from the outside (game logic).
   int mPlantingSpeed = MIN_PLANTING_SPEED;

   PlayerData mData;
   std::vector<std::shared_ptr<Bomb>> mPlantedBombs;
};

#endif // PLAYER_HPP
