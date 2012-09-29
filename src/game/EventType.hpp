#ifndef EVENT_TYPE_HPP
#define EVENT_TYPE_HPP

#include "Arena.hpp"
#include "../utils/Utils.hpp"

#include <vector>

enum class WallType;
enum class ExtraType;
enum class BombType;
enum class ExplosionType;
enum class PlayerType;

/*
 *TODO: May this be more sensible:
 * - RequestCreationBomb
 * - RequestCreationPlayer
 * - ...
 * - RequestRemovalBomb
 * - RequestRemovalPlayer
 * - ...
 * - CreateBomb
 * - CreatePlayer
 * - ...
 * - RemoveBomb
 * - RemovePlayer
 * - ...
 *
 *That would take the animation-switching out of the standard events.
 *Class specific animation event classes could still be created in another file.
 */

enum class EventType
{
   // Create new objects internally
   CreateArena,
   CreateScoreboard,
   CreateWall,
   CreateExtra,
   CreateBomb,
   CreateExplosion,
   CreatePlayer,

   // Spawn animation to show the object on the scene.
   SpawnBombStart,
   SpawnBombEnd,
   SpawnExplosionStart,
   SpawnExplosionEnd,
   SpawnPlayerStart,
   SpawnPlayerEnd,

   // Death animation of the object.
   // DestroyArena,
   // DestroyScoreboard,
   // DestroyWall,
   // DestroyExtra,
   DestroyBombStart,
   DestroyBombEnd,
   DestroyExplosionStart,
   DestroyExplosionEnd,
   DestroyPlayerStart,
   DestroyPlayerEnd,

   // Remove the object from the game logic.
   RemoveBomb,
   RemoveExplosion,
   RemovePlayer,

   Input,

   MoveBomb,
   MovePlayer,

   ParentCellChanged,

   PickupExtra,

   DetonateRemoteBomb,
};

class Event
{
public:
   Event(EventType type, unsigned int sender) : mType(type), mSender(sender) { }
   virtual ~Event() { }

   EventType GetType() const { return mType; }
   unsigned int GetSender() const { return mSender; }

private:
   const EventType mType;
   const unsigned int mSender;
};

class CreateArenaEvent : public Event
{
public:
   CreateArenaEvent(Point pos, Size size, Size boder_size,
                    int cells_x, int cells_y, int player_count)
      : Event(EventType::CreateArena, 0)
      , mPosition(pos)
      , mSize(size)
      , mBoderSize(boder_size)
      , mCellsX(cells_x)
      , mCellsY(cells_y)
      , mPlayerCount(player_count)
   { }
   virtual ~CreateArenaEvent() { }

   Point GetPosition() const { return mPosition; }
   Size GetSize() const { return mSize; }
   Size GetBorderSize() const { return mBoderSize; }
   int GetCellsX() const { return mCellsX; }
   int GetCellsY() const { return mCellsY; }
   int GetPlayerCount() const { return mPlayerCount; }

private:
   const Point mPosition;
   const Size mSize;
   const Size mBoderSize;
   const int mCellsX;
   const int mCellsY;
   const int mPlayerCount;
};

class CreateScoreboardEvent : public Event
{
public:
   CreateScoreboardEvent(Point pos, Size size)
      : Event(EventType::CreateScoreboard, 0)
      , mPosition(pos)
      , mSize(size)
   { }
   virtual ~CreateScoreboardEvent() { }

   Point GetPosition() const { return mPosition; }
   Size GetSize() const { return mSize; }

private:
   const Point mPosition;
   const Size mSize;
};

class CreateWallEvent : public Event
{
public:
   CreateWallEvent(unsigned int sender, const Cell& cell, WallType type)
      : Event(EventType::CreateWall, sender)
      , mCell(cell)
      , mWall(type)
   { }
   virtual ~CreateWallEvent() { }

   Cell GetCell() const { return mCell; }
   WallType GetWall() const { return mWall; }

private:
   const Cell mCell;
   const WallType mWall;
};

class CreateExtraEvent : public Event
{
public:
   CreateExtraEvent(unsigned int sender, const Cell& cell, ExtraType type)
      : Event(EventType::CreateExtra, sender)
      , mCell(cell)
      , mExtra(type)
   { }
   virtual ~CreateExtraEvent() { }

   Cell GetCell() const { return mCell; }
   ExtraType GetExtra() const { return mExtra; }

private:
   const Cell mCell;
   const ExtraType mExtra;
};

class CreateBombEvent : public Event
{
public:
   CreateBombEvent(unsigned int sender, const Cell& cell, BombType type, PlayerType owner, int range)
      : Event(EventType::CreateBomb, sender)
      , mCell(cell)
      , mBombType(type)
      , mOwner(owner)
      , mRange(range)
   { }
   virtual ~CreateBombEvent() { }

   Cell GetCell() const { return mCell; }
   BombType GetBombType() const { return mBombType; }
   int GetRange() const { return mRange; }
   PlayerType GetOwner() const { return mOwner; }

private:
   const Cell mCell;
   const BombType mBombType;
   const PlayerType mOwner;
   const int mRange;
};

class CreateExplosionEvent : public Event
{
public:
   CreateExplosionEvent(unsigned int sender, const Cell& cell, ExplosionType type, PlayerType owner)
      : Event(EventType::CreateExplosion, sender)
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

class CreatePlayerEvent : public Event
{
public:
   CreatePlayerEvent(PlayerType type)
      : Event(EventType::CreatePlayer, 0)
      , mPlayer(type)
   { }
   virtual ~CreatePlayerEvent() { }

   PlayerType GetPlayer() const { return mPlayer; }

private:
   const PlayerType mPlayer;
};



class SpawnBombStartEvent : public Event
{
public:
   SpawnBombStartEvent(unsigned int sender, const Cell& cell, BombType type, PlayerType owner)
      : Event(EventType::SpawnBombStart, sender)
      , mCell(cell)
      , mBombType(type)
      , mOwner(owner)
   { }
   virtual ~SpawnBombStartEvent() { }

   Cell GetCell() const { return mCell; }
   BombType GetBombType() const { return mBombType; }
   PlayerType GetOwner() const { return mOwner; }

private:
   const Cell mCell;
   const BombType mBombType;
   const PlayerType mOwner;
};

class SpawnBombEndEvent : public Event
{
public:
   SpawnBombEndEvent(unsigned int sender)
      : Event(EventType::SpawnBombEnd, sender)
   { }
   virtual ~SpawnBombEndEvent() { }
};


class SpawnExplosionStartEvent : public Event
{
public:
   SpawnExplosionStartEvent(unsigned int sender, const Cell& cell,
                            ExplosionType type, PlayerType owner)
      : Event(EventType::SpawnExplosionStart, sender)
      , mCell(cell)
      , mExplosionType(type)
      , mOwner(owner)
   { }
   virtual ~SpawnExplosionStartEvent() { }

   Cell GetCell() const { return mCell; }
   ExplosionType GetExplosionType() const { return mExplosionType; }
   PlayerType GetOwner() const { return mOwner; }

private:
   const Cell mCell;
   const ExplosionType mExplosionType;
   const PlayerType mOwner;
};

class SpawnExplosionEndEvent : public Event
{
public:
   SpawnExplosionEndEvent(unsigned int sender)
      : Event(EventType::SpawnExplosionEnd, sender)
   { }
   virtual ~SpawnExplosionEndEvent() { }
};


class SpawnPlayerStartEvent : public Event
{
public:
   SpawnPlayerStartEvent(unsigned int sender, PlayerType type)
      : Event(EventType::SpawnPlayerStart, sender)
      , mPlayer(type)
   { }
   virtual ~SpawnPlayerStartEvent() { }

   PlayerType GetPlayer() const { return mPlayer; }

private:
   const PlayerType mPlayer;
};

class SpawnPlayerEndEvent : public Event
{
public:
   SpawnPlayerEndEvent(unsigned int sender)
      : Event(EventType::SpawnPlayerEnd, sender)
   { }
   virtual ~SpawnPlayerEndEvent() { }
};


class DestroyBombStartEvent : public Event
{
public:
   DestroyBombStartEvent(unsigned int sender, const Cell& cell)
      : Event(EventType::DestroyBombStart, sender)
      , mCell(cell)
   { }
   virtual ~DestroyBombStartEvent() { }

   Cell GetCell() const { return mCell; }

private:
   const Cell mCell;
};

class DestroyBombEndEvent : public Event
{
public:
   DestroyBombEndEvent(unsigned int sender, const Cell& cell, PlayerType owner)
      : Event(EventType::DestroyBombEnd, sender)
      , mCell(cell)
      , mOwner(owner)
   { }
   virtual ~DestroyBombEndEvent() { }

   Cell GetCell() const { return mCell; }
   PlayerType GetOwner() const { return mOwner; }

private:
   const Cell mCell;
   const PlayerType mOwner;
};


class DestroyExplosionStartEvent : public Event
{
public:
   DestroyExplosionStartEvent(unsigned int sender)
      : Event(EventType::DestroyExplosionStart, sender)
   { }
   virtual ~DestroyExplosionStartEvent() { }
};

class DestroyExplosionEndEvent : public Event
{
public:
   DestroyExplosionEndEvent(unsigned int sender)
      : Event(EventType::DestroyExplosionEnd, sender)
   { }
   virtual ~DestroyExplosionEndEvent() { }
};


class DestroyPlayerStartEvent : public Event
{
public:
   DestroyPlayerStartEvent(unsigned int sender, PlayerType player, PlayerType killer)
      : Event(EventType::DestroyPlayerStart, sender)
      , mPlayer(player)
      , mKiller(killer)
   { }
   virtual ~DestroyPlayerStartEvent() { }

   PlayerType GetPlayer() const { return mPlayer; }
   PlayerType GetKiller() const { return mKiller; }

private:
   const PlayerType mPlayer;
   const PlayerType mKiller;
};

class DestroyPlayerEndEvent : public Event
{
public:
   DestroyPlayerEndEvent(unsigned int sender)
      : Event(EventType::DestroyPlayerEnd, sender)
   { }
   virtual ~DestroyPlayerEndEvent() { }
};



class RemoveBombEvent : public Event
{
public:
   RemoveBombEvent(unsigned int sender, const Cell& cell)
      : Event(EventType::RemoveBomb, sender)
      , mCell(cell)
   { }
   virtual ~RemoveBombEvent() { }

   Cell GetCell() const { return mCell; }

private:
   const Cell mCell;
};

class RemoveExplosionEvent : public Event
{
public:
   RemoveExplosionEvent(unsigned int sender)
      : Event(EventType::RemoveExplosion, sender)
   { }
   virtual ~RemoveExplosionEvent() { }
};

class RemovePlayerEvent : public Event
{
public:
   RemovePlayerEvent(unsigned int sender)
      : Event(EventType::RemovePlayer, sender)
   { }
   virtual ~RemovePlayerEvent() { }
};



class DetonateRemoteBombEvent : public Event
{
public:
   DetonateRemoteBombEvent(unsigned int sender, PlayerType owner)
      : Event(EventType::DetonateRemoteBomb, sender)
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
      : Event(EventType::Input, 0)
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

class MoveBombEvent : public Event
{
public:
   MoveBombEvent(unsigned int sender, unsigned int bomb, int speed, int distance, Direction dir)
      : Event(EventType::MoveBomb, sender)
      , mBombInstance(bomb)
      , mSpeed(speed)
      , mDistance(distance)
      , mDirection(dir)
   { }
   virtual ~MoveBombEvent() { }

   unsigned int GetBombInstance() const { return mBombInstance; }
   int GetSpeed() const { return mSpeed; }
   int GetDistance() const { return mDistance; }
   Direction GetDirection() const { return mDirection; }

private:
   const unsigned int mBombInstance;
   const int mSpeed;
   const int mDistance;
   const Direction mDirection;
};

class MovePlayerEvent : public Event
{
public:
   MovePlayerEvent(unsigned int sender, const std::vector<std::pair<Direction, int>>& dir)
      : Event(EventType::MovePlayer, sender)
      , mDirectionDistancePair(dir)
   { }
   virtual ~MovePlayerEvent() { }

   std::vector<std::pair<Direction, int>> GetMovementData() const { return mDirectionDistancePair; }

private:
   const std::vector<std::pair<Direction, int>> mDirectionDistancePair;
};

class ParentCellChangedEvent : public Event
{
public:
   ParentCellChangedEvent(unsigned int sender, const Cell& old_parent, const Cell& new_parent)
      : Event(EventType::ParentCellChanged, sender)
      , mOldCell(old_parent)
      , mNewCell(new_parent)
   { }
   virtual ~ParentCellChangedEvent() { }

   Cell GetOldCell() const { return mOldCell; }
   Cell GetNewCell() const { return mNewCell; }

private:
   const Cell mOldCell;
   const Cell mNewCell;
};

class PickupExtraEvent : public Event
{
public:
   PickupExtraEvent(unsigned int sender, const Cell& cell)
      : Event(EventType::PickupExtra, sender)
      , mCell(cell)
   { }
   virtual ~PickupExtraEvent() { }

   Cell GetCell() const { return mCell; }

private:
   const Cell mCell;
};

#endif // EVENT_TYPE_HPP
