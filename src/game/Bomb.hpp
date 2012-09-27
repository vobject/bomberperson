#ifndef BOMB_HPP
#define BOMB_HPP

#include "ArenaObject.hpp"
#include "EventListener.hpp"
#include "../utils/Utils.hpp"

class EventQueue;
class MoveBombEvent;
class CreateExplosionEvent;
class DetonateRemoteBombEvent;

enum class ExplosionType;
enum class PlayerType;

enum class BombType
{
   Countdown,
   Remote
//   CountDownJelly,
//   RemoteJelly
};

enum class BombSound
{
   None,
   Planted
};

class Bomb : public ArenaObject, public EventListener
{
public:
   Bomb(const std::shared_ptr<Arena>& arena,
        BombType type,
        PlayerType owner,
        EventQueue& queue);
   virtual ~Bomb();

   Bomb(const Bomb&) = delete;
   Bomb& operator=(const Bomb&) = delete;

   void Update(int elapsed_time) override;
   void OnEvent(const Event& event) override;

   BombType GetType() const;
   BombSound GetSound(bool reset);

   int GetRange() const;
   void SetRange(int range);

private:
   void OnMoveBomb(const MoveBombEvent& event);
   void OnCreateExplosion(const CreateExplosionEvent& event);
   void OnDetonateRemoteBomb(const DetonateRemoteBombEvent& event);

   void UpdateMovement(int elapsed_time);
   void PlantCenterExplosion() const;
   void PlantRangeExplosion(Direction dir) const;

   bool CanMove(Direction dir, int distance) const;
   ExplosionType GetExplosionType(Direction dir, bool end) const;

   const BombType mType;
   const PlayerType mOwner;

   EventQueue& mEventQueue;

   int mLifeTime = 0_ms;
   int mRange = 1;
   BombSound mSound = BombSound::None;

   bool mIsMoving = false;
   int mMoveIdleTime = 0_ms;

   int mSpeed = 0;
   int mDistance = 0;
   Direction mDirection = Direction::Up;
};

#endif // BOMB_HPP
