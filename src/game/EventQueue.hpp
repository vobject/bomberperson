#ifndef EVENT_QUEUE_HPP
#define EVENT_QUEUE_HPP

#include "EventListener.hpp"
#include "SceneObject.hpp"
#include "Arena.hpp"
#include "../utils/Utils.hpp"

#include <algorithm>
#include <list>
#include <vector>

enum class BombType;
enum class ExplosionType;
enum class PlayerType;

enum class EventType
{
   CreateArena,
   CreateScoreboard,
   CreateWall,
   CreateExtra,
   CreateBomb,
   CreateExplosion,
   CreatePlayer,

   Input,

//   PickupExtra,
//   PlayerDeath,

   MovePlayer,

   DetonateRemoteBomb,
};

class Event
{
public:
   Event(EventType type) : mType(type) { }
   virtual ~Event() { }

   EventType GetType() const { return mType; }

private:
   const EventType mType;
};

class CreateArenaEvent : public Event
{
public:
   CreateArenaEvent(const Cell& cell, BombType type, int range, PlayerType owner)
      : Event(EventType::CreateArena)
      , mCell(cell)
      , mBombType(type)
      , mRange(range)
      , mOwner(owner)
   { }
   virtual ~CreateArenaEvent() { }

   Cell GetCell() const { return mCell; }
   BombType GetBombType() const { return mBombType; }
   int GetRange() const { return mRange; }
   PlayerType GetOwner() const { return mOwner; }

private:
   const Cell mCell;
   const BombType mBombType;
   const int mRange;
   const PlayerType mOwner;
};

class CreateBombEvent : public Event
{
public:
   CreateBombEvent(const Cell& cell, BombType type, int range, PlayerType owner)
      : Event(EventType::CreateBomb)
      , mCell(cell)
      , mBombType(type)
      , mRange(range)
      , mOwner(owner)
   { }
   virtual ~CreateBombEvent() { }

   Cell GetCell() const { return mCell; }
   BombType GetBombType() const { return mBombType; }
   int GetRange() const { return mRange; }
   PlayerType GetOwner() const { return mOwner; }

private:
   const Cell mCell;
   const BombType mBombType;
   const int mRange;
   const PlayerType mOwner;
};

class CreateExplosionEvent : public Event
{
public:
   CreateExplosionEvent(const Cell& cell, ExplosionType type, PlayerType owner)
      : Event(EventType::CreateExplosion)
      , mCell(cell)
      , mExplosionType(type)
      , mOwner(owner)
   { }
   virtual ~CreateExplosionEvent() { }

   Cell GetCell() const { return mCell; }
   ExplosionType GetExplosionType() const { return mExplosionType; }
   PlayerType GetOwner() const { return mOwner; }

private:
   const Cell mCell;
   const ExplosionType mExplosionType;
   const PlayerType mOwner;
};

class DetonateRemoteBombEvent : public Event
{
public:
   DetonateRemoteBombEvent(PlayerType owner)
      : Event(EventType::DetonateRemoteBomb)
      , mOwner(owner)
   { }
   virtual ~DetonateRemoteBombEvent() { }

   PlayerType GetOwner() const { return mOwner; }

private:
   const PlayerType mOwner;
};

class InputEvent : public Event
{
public:
   InputEvent(PlayerType player, bool up, bool down,
                                 bool left, bool right,
                                 bool action1, bool action2)
      : Event(EventType::Input)
      , mPlayer(player)
      , mUp(up)
      , mDown(down)
      , mLeft(left)
      , mRight(right)
      , mAction1(action1)
      , mAction2(action2)
   { }
   virtual ~InputEvent() { }

   PlayerType GetPlayerType() const { return mPlayer; }
   bool GetUp() const { return mUp; }
   bool GetDown() const { return mDown; }
   bool GetLeft() const { return mLeft; }
   bool GetRight() const { return mRight; }
   bool GetAction1() const { return mAction1; }
   bool GetAction2() const { return mAction2; }

private:
   const PlayerType mPlayer;
   const bool mUp;
   const bool mDown;
   const bool mLeft;
   const bool mRight;
   const bool mAction1;
   const bool mAction2;
};

class MovePlayerEvent : public Event
{
public:
   MovePlayerEvent(PlayerType player, const std::vector<std::pair<Direction, int>>& dir)
      : Event(EventType::MovePlayer)
      , mPlayer(player)
      , mDirectionDistancePair(dir)
   { }
   virtual ~MovePlayerEvent() { }

   PlayerType GetPlayerType() const { return mPlayer; }
   std::vector<std::pair<Direction, int>> GetMovementData() const { return mDirectionDistancePair; }

private:
   const PlayerType mPlayer;
   const std::vector<std::pair<Direction, int>> mDirectionDistancePair;
};

class EventQueue
{
public:
   EventQueue();
   ~EventQueue();

   void Register(EventListener* obj);
   void UnRegister(EventListener* obj);

   void Add(const std::shared_ptr<Event>& event);

   void ProcessEvents();

private:
   void SwapQueues();

   // Objects (basically all) that want to know about enqueued events.
   std::list<EventListener*> mListeners;

   static const int QUEUE_COUNT = 2;
   std::vector<std::shared_ptr<Event>> mQueues[QUEUE_COUNT];
   int mCurrentQueue = 0;
};

#endif // EVENT_QUEUE_HPP
