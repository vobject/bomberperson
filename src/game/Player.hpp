#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "ArenaObject.hpp"
#include "../input/InputDevice.hpp"
#include "../utils/Utils.hpp"

#include <memory>
#include <vector>

class EntityManager;
class Bomb;

enum class PlayerType
{
   Player_1,
   Player_2,
   Player_3,
   Player_4
};

enum class PlayerSound
{
   None,
   Collect_Speed,
   Collect_Bombs,
   Collect_Range,
   Collect_InfiniteRange,
   Die
//   Win
};

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
   Dying
//   Win
};

struct PlayerData
{
   constexpr PlayerData(const PlayerAnimation anim,
                        const int speed,
                        const int distance)
      : anim(anim)
      , speed(speed)
      , distance(distance)
      , bombs(1)
      , range(1)
      , remote_bombs(0)
      , kills(0)
      , wins(0)
   { }

   PlayerAnimation anim;

   int speed;
   int distance;
   int bombs;
   int range;
   int remote_bombs;

   int kills;
   int wins;
};

class Player : public ArenaObject
{
public:
   Player(const std::shared_ptr<Arena>& arena,
          PlayerType type,
          EntityManager& entity_factory);
   virtual ~Player();

   Player(const Player&) = delete;
   Player& operator=(const Player&) = delete;

   void Update(int elapsed_time) override;

   void SetInputCommands(InputCommands cmds);

   PlayerType GetType() const;
   PlayerData GetData() const;
   PlayerSound GetSound(bool reset);

   void IncrementKills(PlayerType type);

private:
   // Number of milliseconds the player has to wait to move another pixel.
   static const int MIN_SPEED = 12_ms;
   static const int MAX_SPEED = 2_ms;

   // Number of milliseconds the player has to wait to plant another bomb.
   static const int MIN_PLANTING_SPEED = 250_ms;

   void UpdateMovement(int elapsed_time);
   void UpdateBombing(int elapsed_time);
   void UpdateAnimation(int elapsed_time);

   bool CanMove(Direction dir, int distance) const;
   bool CanPlantBomb();

   void IncreaseSpeed();
   PlayerAnimation GetStopWalkingState(PlayerAnimation anim) const;

   PlayerType mType;
   PlayerData mData;
   PlayerSound mSound;

   EntityManager& mEntityFactory;

   InputCommands mCurrentCommands;

   int mMoveIdleTime = 0_ms;
   int mBombIdleTime = 0_ms;

   // Move this into the PlayerData object if we choose to make
   //  it alterable from the outside (game logic).
   int mPlantingSpeed = MIN_PLANTING_SPEED;
   std::vector<std::shared_ptr<Bomb>> mPlantedBombs;
};

#endif // PLAYER_HPP
