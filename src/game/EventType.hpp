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
   SpawnPlayerStart,
   SpawnPlayerEnd,

   // Death animation of the object.
   // DestroyArena,
   // DestroyScoreboard,
   // DestroyWall,
   // DestroyExtra,
//   DestroyBomb,
//   DestroyExplosion,
   // DestroyPlayer,
   DestroyPlayerStart,
   DestroyPlayerEnd,

   // Remove the object from the game logic.
   RemovePlayer,

   Input,

   MovePlayer,

   PickupExtra,

   DetonateRemoteBomb
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
   CreateArenaEvent(Point pos, Size size, Size boder_size,
                    int cells_x, int cells_y, int player_count)
      : Event(EventType::CreateArena)
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
      : Event(EventType::CreateScoreboard)
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
   CreateWallEvent(const Cell& cell, WallType type)
      : Event(EventType::CreateWall)
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
   CreateExtraEvent(const Cell& cell, ExtraType type)
      : Event(EventType::CreateExtra)
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

class CreatePlayerEvent : public Event
{
public:
   CreatePlayerEvent(PlayerType type)
      : Event(EventType::CreatePlayer)
      , mPlayer(type)
   { }
   virtual ~CreatePlayerEvent() { }

   PlayerType GetPlayer() const { return mPlayer; }

private:
   const PlayerType mPlayer;
};

class SpawnPlayerStartEvent : public Event
{
public:
   SpawnPlayerStartEvent(PlayerType type)
      : Event(EventType::SpawnPlayerStart)
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
   SpawnPlayerEndEvent(PlayerType type)
      : Event(EventType::SpawnPlayerEnd)
      , mPlayer(type)
   { }
   virtual ~SpawnPlayerEndEvent() { }

   PlayerType GetPlayer() const { return mPlayer; }

private:
   const PlayerType mPlayer;
};

class DestroyPlayerStartEvent : public Event
{
public:
   DestroyPlayerStartEvent(PlayerType player, PlayerType killer)
      : Event(EventType::DestroyPlayerStart)
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
   DestroyPlayerEndEvent(PlayerType player)
      : Event(EventType::DestroyPlayerEnd)
      , mPlayer(player)
   { }
   virtual ~DestroyPlayerEndEvent() { }

   PlayerType GetPlayer() const { return mPlayer; }

private:
   const PlayerType mPlayer;
};

class RemovePlayerEvent : public Event
{
public:
   RemovePlayerEvent(PlayerType player)
      : Event(EventType::RemovePlayer)
      , mPlayer(player)
   { }
   virtual ~RemovePlayerEvent() { }

   PlayerType GetPlayer() const { return mPlayer; }

private:
   const PlayerType mPlayer;
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

   PlayerType GetPlayer() const { return mPlayer; }
   std::vector<std::pair<Direction, int>> GetMovementData() const { return mDirectionDistancePair; }

private:
   const PlayerType mPlayer;
   const std::vector<std::pair<Direction, int>> mDirectionDistancePair;
};

class PickupExtraEvent : public Event
{
public:
   PickupExtraEvent(PlayerType player, const Cell& cell)
      : Event(EventType::PickupExtra)
      , mPlayer(player)
      , mCell(cell)
   { }
   virtual ~PickupExtraEvent() { }

   PlayerType GetPlayer() const { return mPlayer; }
   Cell GetCell() const { return mCell; }

private:
   const PlayerType mPlayer;
   const Cell mCell;
};

#endif // EVENT_TYPE_HPP
