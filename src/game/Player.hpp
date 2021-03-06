#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "ArenaObject.hpp"
#include "EventListener.hpp"
#include "../input/InputDevice.hpp"
#include "../utils/Utils.hpp"

#include <memory>
#include <vector>

class EventQueue;
class RemovePlayerEvent;
class InputEvent;
class RemoveBombEvent;
class KillPlayerEvent;
struct Cell;

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
   Spawn,
   Destroy
//   Win
};

// TODO: Should belong to some sort of RenderInfo data structure.
//  Because it is not really useful for the game logic.
enum class PlayerAnimation
{
   Spawn,
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
//   Win,
   Destroy
};

class Player : public ArenaObject, public EventListener
{
public:
   Player(const std::shared_ptr<Arena>& arena,
          PlayerType type,
          int spawn_len,
          int death_len,
          EventQueue& queue);
   virtual ~Player();

   Player(const Player&) = delete;
   Player& operator=(const Player&) = delete;

   void Update(int elapsed_time) override;
   void OnEvent(const Event& event) override;

   PlayerType GetType() const;
   PlayerAnimation GetAnimation() const;
   int GetSpeed() const;
   int GetDistance() const;
   PlayerSound GetSound(bool reset);

private:
   void OnRemovePlayer(const RemovePlayerEvent& event);
   void OnInput(const InputEvent& event);
   void OnRemoveBomb(const RemoveBombEvent& event);
   void OnKillPlayer(const KillPlayerEvent& event);

   // Number of milliseconds the player has to wait to move another pixel.
   static const int MIN_SPEED = 12_ms;
   static const int MAX_SPEED = 2_ms;

   // Number of milliseconds the player has to wait to plant another bomb.
   static const int MIN_PLANTING_SPEED = 250_ms;

   void PickupExtra(Cell cell);
   void UpdateMovement(int elapsed_time);
   void UpdateBombing(int elapsed_time);
   void UpdateAnimation(int elapsed_time);

   bool CanMove(Direction dir, int distance) const;
   bool CanPlantBomb();

   void IncreaseSpeed();
   PlayerAnimation GetStopWalkingState(PlayerAnimation anim) const;

   const PlayerType mType;
   const int mSpawnAnimLength;
   const int mDeathAnimLength;

   EventQueue& mEventQueue;

   // Input handling:
   bool mInputUp = false;
   bool mInputDown = false;
   bool mInputLeft = false;
   bool mInputRight = false;
   bool mInputAction1 = false;
   bool mInputAction2 = false;

   // Animation:
   PlayerAnimation mAnimation = PlayerAnimation::StandDown;

   // Sound:
   PlayerSound mSound = PlayerSound::None;

   // Powerups:
   int mSpeed = MIN_SPEED;
   int mDistance = 1;
   int mBombSupply = 1;
   int mBombRange = 1;
   int mRemoteBombs = 0;
   bool mCanKick = false;

   // Statistics:
   int mBombsPlanted = 0;
   std::vector<PlayerType> mKills;

   int mMoveIdleTime = 0_ms;
   int mBombIdleTime = 0_ms;

   // Move this into the PlayerData object if we choose to make
   //  it alterable from the outside (game logic).
   int mPlantingSpeed = MIN_PLANTING_SPEED;
};

#endif // PLAYER_HPP
