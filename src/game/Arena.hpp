#ifndef ARENA_HPP
#define ARENA_HPP

#include "SceneObject.hpp"

#include <memory>
#include <vector>

class ArenaObject;
class Wall;
class Extra;
class Bomb;
class Explosion;
struct Point;
struct Size;

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

   constexpr bool operator==(const Cell& other)
   { return ((X == other.X) && (Y == other.Y)); }

   int X;
   int Y;
};

class Arena : public SceneObject
{
public:
   Arena(const Point& pos, const Size& size, const Size& borders,
         int cells_x, int cells_y);
   virtual ~Arena();

   Arena(const Arena&) = delete;
   Arena& operator=(const Arena&) = delete;

   void Update(int elapsed_time) override;

   // TODO?
   // GetBorderSize();
   // GetDimensions();

   void SetObjectPosition(ArenaObject& obj, const Cell& cell) const;
   void SetObjectSize(ArenaObject& obj) const;

   Size GetCellSize() const;
   Point GetCellPosition(const Cell& cell) const;

   Cell GetCellFromObject(const ArenaObject& obj) const;
   Cell GetCellFromPosition(const Point& pos) const;
   Cell GetCellFromCoordinates(int cell_x, int cell_y) const;

   Cell GetNeighborCell(const Cell& cell, Direction dir) const;

   bool HasWall(const Cell& cell) const;
   std::shared_ptr<Wall> GetWall(const Cell& cell) const;
   void SetWall(const Cell& cell, const std::shared_ptr<Wall>& wall);
   void DestroyWall(const Cell& cell);

   bool HasExtra(const Cell& cell) const;
   std::shared_ptr<Extra> GetExtra(const Cell& cell) const;
   void SetExtra(const Cell& cell, const std::shared_ptr<Extra>& extra);

   bool HasBomb(const Cell& cell) const;
   std::shared_ptr<Bomb> GetBomb(const Cell& cell) const;
   void SetBomb(const Cell& cell, const std::shared_ptr<Bomb>& bomb);

   bool HasExplosion(const Cell& cell) const;
   std::shared_ptr<Explosion> GetExplosion(const Cell& cell) const;
   void SetExplosion(const Cell& cell, const std::shared_ptr<Explosion>& explosion);

private:
   struct CellContent
   {
      std::shared_ptr<Wall> wall;
      std::shared_ptr<Extra> extra;
      std::shared_ptr<Bomb> bomb;
      std::shared_ptr<Explosion> explosion;
//      std::vector<std::shared_ptr<Player>> players;
   };

   Cell GetCellAboveOf(int cell_x, int cell_y) const;
   Cell GetCellBelowOf(int cell_x, int cell_y) const;
   Cell GetCellLeftOf(int cell_x, int cell_y) const;
   Cell GetCellRightOf(int cell_x, int cell_y) const;

   const int mXCells; // Number of horizontal cells.
   const int mYCells; // Number of vertical cells.
   const Size mBorders; // Size of the arenas borders in pixel.
   const Size mCellSize;
   std::vector<std::vector<std::pair<Cell, CellContent>>> mCells;
};

#endif // ARENA_HPP
