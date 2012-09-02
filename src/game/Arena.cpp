#include "Arena.hpp"
#include "Cell.hpp"
#include "../utils/Utils.hpp"

Arena::Arena()
   : SceneObject(EntityId::Arena)
{

}

Arena::~Arena()
{

}

void Arena::Update(const int elapsed_time)
{
   // TODO: May also be used for an animated ground (e.g. grass or craters).

   for (auto& cell : mCells)
   {
      cell->Update(elapsed_time);
   }
}

void Arena::SetBorderSize(const Size& borders)
{
   mBorders = borders;
}

void Arena::SetDimensions(const int cells_x, const int cells_y)
{
   mXCells = cells_x;
   mYCells = cells_y;
}

void Arena::SetCells(const std::vector<std::shared_ptr<Cell>>& cells)
{
   mCells = cells;
}

std::vector<std::shared_ptr<Cell>> Arena::GetCells() const
{
   return mCells;
}

std::shared_ptr<Cell> Arena::GetCellFromPosition(const Point& pos) const
{
   const Size cell_size((GetSize().Width - (mBorders.Width * 2)) / mXCells,
                        (GetSize().Height - (mBorders.Height * 2)) / mYCells);

   const auto cell_x = (pos.X - GetPosition().X - mBorders.Width) / cell_size.Width;
   const auto cell_y = (pos.Y - GetPosition().Y - mBorders.Height) / cell_size.Height;
   return GetCellFromCoordinates(cell_x, cell_y);
}

std::shared_ptr<Cell> Arena::GetCellFromCoordinates(const int cell_x, const int cell_y) const
{
   const auto index = ArenaPosToIndex(cell_x, cell_y);
   if (index >= mCells.size()) {
      return nullptr;
   }
   return mCells[index];
}

std::shared_ptr<Cell> Arena::GetCellAboveOf(const int cell_x, const int cell_y) const
{
   if (cell_y <= 0) {
      return nullptr;
   }
   return GetCellFromCoordinates(cell_x, cell_y - 1);
}

std::shared_ptr<Cell> Arena::GetCellBelowOf(const int cell_x, const int cell_y) const
{
   return GetCellFromCoordinates(cell_x, cell_y + 1);
}

std::shared_ptr<Cell> Arena::GetCellLeftOf(const int cell_x, const int cell_y) const
{
   if (cell_x <= 0) {
      return nullptr;
   }
   return GetCellFromCoordinates(cell_x - 1, cell_y);
}

std::shared_ptr<Cell> Arena::GetCellRightOf(const int cell_x, const int cell_y) const
{
   if (cell_x == (mXCells - 1)) {
      return nullptr;
   }
   return GetCellFromCoordinates(cell_x + 1, cell_y);
}

std::tuple<int, int> Arena::IndexToArenaPos(const int index) const
{
   return std::make_tuple(index % mYCells, index / mYCells);
}

int Arena::ArenaPosToIndex(const int cell_x, const int cell_y) const
{
   return (cell_x + (cell_y * mXCells));
}
