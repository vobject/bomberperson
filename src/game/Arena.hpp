#ifndef ARENA_HPP
#define ARENA_HPP

#include "SceneObject.hpp"

#include <memory>
#include <tuple>
#include <vector>

class Cell;
struct Point;
struct Size;

class Arena : public SceneObject
{
public:
   Arena();
   virtual ~Arena();

   Arena(const Arena&) = delete;
   Arena& operator=(const Arena&) = delete;

   void Update(int elapsed_time) override;

   void SetBorderSize(const Size& borders);
   void SetDimensions(int cells_x, int cells_y);
   Size GetCellSize() const;

   std::vector<std::shared_ptr<Cell>> GetCells() const;
   void SetCells(const std::vector<std::shared_ptr<Cell>>& cells);

   std::shared_ptr<Cell> GetCellFromPosition(const Point& pos) const;
   std::shared_ptr<Cell> GetCellFromCoordinates(int cell_x, int cell_y) const;

   std::shared_ptr<Cell> GetCellAboveOf(int cell_x, int cell_y) const;
   std::shared_ptr<Cell> GetCellBelowOf(int cell_x, int cell_y) const;
   std::shared_ptr<Cell> GetCellLeftOf(int cell_x, int cell_y) const;
   std::shared_ptr<Cell> GetCellRightOf(int cell_x, int cell_y) const;

private:
   std::tuple<int, int> IndexToArenaPos(int index) const;
   int ArenaPosToIndex(int cell_x, int cell_y) const;

   Size mBorders = { 0, 0 };
   int mXCells = 0;
   int mYCells = 0;
   std::vector<std::shared_ptr<Cell>> mCells;
};

#endif // ARENA_HPP
