#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "ArenaObject.hpp"
#include "EventListener.hpp"
#include "EventQueue.hpp"
#include "../input/InputDevice.hpp"
#include "../utils/Utils.hpp"

#include <memory>
#include <vector>

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
   Spawn,
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
      , remote_bombs(1)
      , can_kick(false)
      , kills(0)
      , wins(0)
   { }

   PlayerAnimation anim;

   int speed;
   int distance;
   int bombs;
   int range;
   int remote_bombs;
   bool can_kick;

   int kills;
   int wins;
};

class Player : public ArenaObject, public EventListener
{
public:
   Player(const std::shared_ptr<Arena>& arena,
          PlayerType type,
          EventQueue& queue);
   virtual ~Player();

   Player(const Player&) = delete;
   Player& operator=(const Player&) = delete;

   void Update(int elapsed_time) override;
   void OnEvent(const Event& event) override;

   PlayerType GetType() const;
   PlayerData GetData() const;
   PlayerSound GetSound(bool reset);

//   void IncrementKills(PlayerType type);

protected:
   void OnCreateBomb(const CreateBombEvent& event);
   void OnCreateExplosion(const CreateExplosionEvent& event);
   void OnInput(const InputEvent& event);
   void OnMovePlayer(const MovePlayerEvent& event);

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
//   void KickBomb(Direction dir) const;

   void IncreaseSpeed();
   PlayerAnimation GetStopWalkingState(PlayerAnimation anim) const;
   Direction GetPlayerDirection() const;

   PlayerType mType;
   PlayerData mData;
   PlayerSound mSound;

   EventQueue& mEventQueue;

//   InputCommands mCurrentCommands;
   bool mInputUp;
   bool mInputDown;
   bool mInputLeft;
   bool mInputRight;
   bool mInputAction1;
   bool mInputAction2;

   int mMoveIdleTime = 0_ms;
   int mBombIdleTime = 0_ms;

   // Move this into the PlayerData object if we choose to make
   //  it alterable from the outside (game logic).
   int mPlantingSpeed = MIN_PLANTING_SPEED;
//   std::vector<std::shared_ptr<Bomb>> mPlantedBombs;
   int mBombsPlanted = 0;

   // HACK: Handle this properly!
//   mutable bool mParentCellChanged = false;
};

#endif // PLAYER_HPP
