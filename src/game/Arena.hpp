#ifndef ARENA_HPP
#define ARENA_HPP

#include "SceneObject.hpp"
#include "EventListener.hpp"

#include <list>
#include <map>
#include <memory>
#include <vector>

class EventQueue;

class RemoveArenaEvent;
class CreateWallEvent;
class CreateExtraEvent;
class CreateBombEvent;
class CreateExplosionEvent;
class RemoveWallEvent;
class RemoveExtraEvent;
class RemoveBombEvent;
class RemoveExplosionEvent;
class BombCellChangedEvent;

class ArenaObject;
class Wall;
class Extra;
class Bomb;
class Player;
struct Point;
struct Size;

enum class WallType;
enum class ExtraType;
enum class BombType;
enum class PlayerType;

//enum class ArenaType
//{

//};

enum class ArenaMusic
{
   None,
   DefaultTrack
};

struct Cell
{
   constexpr Cell(const int x, const int y) : X(x), Y(y) { }

   constexpr bool operator==(const Cell other)
   { return ((X == other.X) && (Y == other.Y)); }

   constexpr bool operator!=(const Cell other)
   { return !(*this == other); }

   int X;
   int Y;
};

class Arena : public SceneObject, public EventListener
{
public:
   Arena(const Point pos, const Size size, const Size borders,
         int cells_x, int cells_y, EventQueue& queue);
   virtual ~Arena();

   Arena(const Arena&) = delete;
   Arena& operator=(const Arena&) = delete;

   void Update(int elapsed_time) override;
   void OnEvent(const Event& event) override;

   void SetObjectPosition(ArenaObject& obj, const Cell& cell) const;
   void SetObjectSize(ArenaObject& obj) const;

   Size GetCellSize() const;
   Point GetCellPosition(const Cell& cell) const;

   Cell GetCellFromObject(const ArenaObject& obj) const;
   Cell GetCellFromPosition(const Point& pos) const;
   Cell GetCellFromCoordinates(int cell_x, int cell_y) const;

   Cell GetNeighborCell(const Cell& cell, Direction dir) const;

   bool HasWall(Cell cell) const;
   WallType GetWallType(Cell cell) const;

   bool HasExtra(Cell cell) const;
   ExtraType GetExtraType(Cell cell) const;

   bool HasBomb(Cell cell) const;
   BombType GetBombType(Cell cell) const;

   bool HasExplosion(Cell cell) const;
   PlayerType GetExplosionOwner(Cell cell) const;

private:
   void OnRemoveArena(const RemoveArenaEvent& event);

   void OnCreateWall(const CreateWallEvent& event);
   void OnCreateExtra(const CreateExtraEvent& event);
   void OnCreateBomb(const CreateBombEvent& event);
   void OnCreateExplosion(const CreateExplosionEvent& event);

   void OnRemoveWall(const RemoveWallEvent& event);
   void OnRemoveExtra(const RemoveExtraEvent& event);
   void OnRemoveBomb(const RemoveBombEvent& event);
   void OnRemoveExplosion(const RemoveExplosionEvent& event);

   void OnBombCellChanged(const BombCellChangedEvent& event);

   Cell GetCellAboveOf(int cell_x, int cell_y) const;
   Cell GetCellBelowOf(int cell_x, int cell_y) const;
   Cell GetCellLeftOf(int cell_x, int cell_y) const;
   Cell GetCellRightOf(int cell_x, int cell_y) const;

   void CreateWall(Cell cell, WallType type);
   void CreateExtra(Cell cell, ExtraType type);
   void CreateBomb(Cell cell, BombType type);
   void CreateExplosion(Cell cell, PlayerType owner);

   void RemoveWall(Cell cell);
   void RemoveExtra(Cell cell);
   void RemoveBomb(Cell cell);
   void RemoveExplosion(Cell cell);

   struct CellContent
   {
      bool wall = false;
      bool extra = false;
      bool bomb = false;
      bool explosion = false;

      WallType wall_type;
      ExtraType extra_type;
      BombType bomb_type;
      PlayerType explosion_owner;
   };

   const int mXCells; // Number of horizontal cells.
   const int mYCells; // Number of vertical cells.
   const Size mBorders; // Size of the arenas borders in pixel.
   const Size mCellSize;
   std::vector<std::vector<std::pair<Cell, CellContent>>> mCells;

   EventQueue& mEventQueue;
};

#endif // ARENA_HPP
