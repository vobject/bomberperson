#ifndef EVENT_TYPE_HPP
#define EVENT_TYPE_HPP

#include "Arena.hpp"
#include "../utils/Utils.hpp"

enum class ArenaType;
enum class WallType;
enum class ExtraType;
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

   RemoveArena,
   RemoveScoreboard,
   RemoveWall,
   RemoveExtra,
   RemoveBomb,
   RemoveExplosion,
   RemovePlayer,

   Input,               // From Match to the players.
   KillPlayer,          // From Player to other players and itself.
   DetonateBomb,        // From Bomb to other bombs to get a chain reaction.
   DetonateRemoteBomb,  // From Player to bombs.
   BombCellChanged,     // From Bomb to Arena when bomb is moving.
   MoveBomb             // From Player on kick and from Bomb itself when moving.
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
   CreateArenaEvent(ArenaType type, Point pos, Size size, Size boder_size,
                    int cells_x, int cells_y, int player_count)
      : Event(EventType::CreateArena, 0)
      , mArena(type)
      , mPosition(pos)
      , mSize(size)
      , mBoderSize(boder_size)
      , mCellsX(cells_x)
      , mCellsY(cells_y)
      , mPlayerCount(player_count)
   { }
   virtual ~CreateArenaEvent() { }

   ArenaType GetArena() const { return mArena; }
   Point GetPosition() const { return mPosition; }
   Size GetSize() const { return mSize; }
   Size GetBorderSize() const { return mBoderSize; }
   int GetCellsX() const { return mCellsX; }
   int GetCellsY() const { return mCellsY; }
   int GetPlayerCount() const { return mPlayerCount; }

private:
   const ArenaType mArena;
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
   CreateWallEvent(unsigned int sender, Cell cell, const WallType type)
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
   CreateExtraEvent(unsigned int sender, Cell cell, const ExtraType type)
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
   CreateBombEvent(unsigned int sender, Cell cell, BombType type, PlayerType owner, int range)
      : Event(EventType::CreateBomb, sender)
      , mCell(cell)
      , mBomb(type)
      , mOwner(owner)
      , mRange(range)
   { }
   virtual ~CreateBombEvent() { }

   Cell GetCell() const { return mCell; }
   BombType GetBomb() const { return mBomb; }
   int GetRange() const { return mRange; }
   PlayerType GetOwner() const { return mOwner; }

private:
   const Cell mCell;
   const BombType mBomb;
   const PlayerType mOwner;
   const int mRange;
};

class CreateExplosionEvent : public Event
{
public:
   CreateExplosionEvent(unsigned int sender, Cell cell, ExplosionType type, PlayerType owner)
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

class RemoveArenaEvent : public Event
{
public:
   RemoveArenaEvent()
      : Event(EventType::RemoveArena, 0)
   { }
   virtual ~RemoveArenaEvent() { }
};

class RemoveScoreboardEvent : public Event
{
public:
   RemoveScoreboardEvent()
      : Event(EventType::RemoveScoreboard, 0)
   { }
   virtual ~RemoveScoreboardEvent() { }
};

class RemoveWallEvent : public Event
{
public:
   RemoveWallEvent(unsigned int sender, Cell cell)
      : Event(EventType::RemoveWall, sender)
      , mCell(cell)
   { }
   virtual ~RemoveWallEvent() { }

   Cell GetCell() const { return mCell; }

private:
   const Cell mCell;
};

class RemoveExtraEvent : public Event
{
public:
   RemoveExtraEvent(unsigned int sender, Cell cell)
      : Event(EventType::RemoveExtra, sender)
      , mCell(cell)
   { }
   virtual ~RemoveExtraEvent() { }

   Cell GetCell() const { return mCell; }

private:
   const Cell mCell;
};

class RemoveBombEvent : public Event
{
public:
   RemoveBombEvent(unsigned int sender, Cell cell, PlayerType owner)
      : Event(EventType::RemoveBomb, sender)
      , mCell(cell)
      , mOwner(owner)
   { }
   virtual ~RemoveBombEvent() { }

   Cell GetCell() const { return mCell; }
   PlayerType GetOwner() const { return mOwner; }

private:
   const Cell mCell;
   const PlayerType mOwner;
};

class RemoveExplosionEvent : public Event
{
public:
   RemoveExplosionEvent(unsigned int sender, Cell cell)
      : Event(EventType::RemoveExplosion, sender)
      , mCell(cell)
   { }
   virtual ~RemoveExplosionEvent() { }

   Cell GetCell() const { return mCell; }

private:
   const Cell mCell;
};

class RemovePlayerEvent : public Event
{
public:
   RemovePlayerEvent(unsigned int sender, PlayerType type)
      : Event(EventType::RemovePlayer, sender)
      , mPlayer(type)
   { }
   virtual ~RemovePlayerEvent() { }

   PlayerType GetPlayer() const { return mPlayer; }

private:
   const PlayerType mPlayer;
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

class KillPlayerEvent : public Event
{
public:
   KillPlayerEvent(unsigned int sender, PlayerType victim, PlayerType killer)
      : Event(EventType::KillPlayer, sender)
      , mVictim(victim)
      , mKiller(killer)
   { }
   virtual ~KillPlayerEvent() { }

   PlayerType GetVictim() const { return mVictim; }
   PlayerType GetKiller() const { return mKiller; }

private:
   const PlayerType mVictim;
   const PlayerType mKiller;
};

class DetonateBombEvent : public Event
{
public:
   DetonateBombEvent(unsigned int sender, Cell cell)
      : Event(EventType::DetonateBomb, sender)
      , mCell(cell)
   { }
   virtual ~DetonateBombEvent() { }

   Cell GetCell() const { return mCell; }

private:
   const Cell mCell;
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

class BombCellChangedEvent : public Event
{
public:
   BombCellChangedEvent(unsigned int sender, const Cell& old_parent, const Cell& new_parent)
      : Event(EventType::BombCellChanged, sender)
      , mOldCell(old_parent)
      , mNewCell(new_parent)
   { }
   virtual ~BombCellChangedEvent() { }

   Cell GetOldCell() const { return mOldCell; }
   Cell GetNewCell() const { return mNewCell; }

private:
   const Cell mOldCell;
   const Cell mNewCell;
};

class MoveBombEvent : public Event
{
public:
   MoveBombEvent(unsigned int sender, Cell cell, int speed, int distance, Direction dir)
      : Event(EventType::MoveBomb, sender)
      , mCell(cell)
      , mSpeed(speed)
      , mDistance(distance)
      , mDirection(dir)
   { }
   virtual ~MoveBombEvent() { }

   Cell GetCell() const { return mCell; }
   int GetSpeed() const { return mSpeed; }
   int GetDistance() const { return mDistance; }
   Direction GetDirection() const { return mDirection; }

private:
   const Cell mCell;
   const int mSpeed;
   const int mDistance;
   const Direction mDirection;
};

#endif // EVENT_TYPE_HPP
