#include "ArenaGenerator.hpp"
#include "EntityManager.hpp"
#include "Arena.hpp"
#include "Cell.hpp"
#include "Wall.hpp"
#include "Extra.hpp"
#include "../Options.hpp"

#include <cstdlib>
#include <ctime>

ArenaGenerator::ArenaGenerator(EntityManager& entity_factory)
   : mEntityFactory(entity_factory)
{
   srand(time(nullptr));
}

ArenaGenerator::~ArenaGenerator()
{

}

void ArenaGenerator::SetArenaPosition(const Point& pos)
{
   mPos = pos;
}

void ArenaGenerator::SetArenaSize(const Size& size)
{
   mSize = size;
}

void ArenaGenerator::SetArenaBorderSize(const Size& borders)
{
   mBorders = borders;
}

std::shared_ptr<Arena> ArenaGenerator::GetDefaultArena(
   const int cells_x,
   const int cells_y,
   const int players
) const
{
   if (!mSize) {
      throw "Arena size not yet set.";
   }

   auto arena = std::make_shared<Arena>();
   arena->SetPosition(mPos);
   arena->SetSize(mSize);
   arena->SetBorderSize(mBorders);
   arena->SetDimensions(cells_x, cells_y);

   const auto cells = CreateDefaultCells(cells_x, cells_y, arena);

   // CreateDefaultCells() put a wall at every cell.
   // Now clean up the cells that we want to put players on.

   if (players >= 1)
   {
      cells[0]->GetWall()->SetAlive(false);
      cells[1]->GetWall()->SetAlive(false);
      cells[cells_x]->GetWall()->SetAlive(false);
   }

   if (players >= 2)
   {
      cells[cells_x * cells_y - 1]->GetWall()->SetAlive(false);
      cells[cells_x * cells_y - 2]->GetWall()->SetAlive(false);
      cells[cells_x * cells_y - cells_x - 1]->GetWall()->SetAlive(false);
   }

   if (players >= 3)
   {
      cells[cells_x * cells_y - cells_x * 2]->GetWall()->SetAlive(false);
      cells[cells_x * cells_y - cells_x]->GetWall()->SetAlive(false);
      cells[cells_x * cells_y - cells_x + 1]->GetWall()->SetAlive(false);
   }

   if (players >= 4)
   {
      cells[cells_x - 2]->GetWall()->SetAlive(false);
      cells[cells_x - 1]->GetWall()->SetAlive(false);
      cells[cells_x * 2 - 1]->GetWall()->SetAlive(false);
   }

   for (auto& cell : cells)
   {
      if (!cell->HasWall()) {
         // Do not put extras on a plain cell without a wall.
         continue;
      }

      if (!cell->GetWall()->IsAlive()) {
         // Do not put extras on a cell whose wall is no longer alive.
         continue;
      }

      if (!cell->GetWall()->IsDestructible()) {
         // Do not put extras on cells with an indestructible wall.
         continue;
      }

      if (!ShouldCreateItem()) {
         // Do not put extras on the cell if Fortuna is not with us.
         continue;
      }

      std::shared_ptr<Extra> extra;

      if (!(rand() % (cells_x * cells_y / 3)))
      {
         // Very little chance for a golden flame (infinite explosion range).
         extra = mEntityFactory.CreateExtra(EntityId::GoldRangeExtra, cell);
      }
      else
      {
         switch (rand() % 3)
         {
            case 0:
               extra = mEntityFactory.CreateExtra(EntityId::SpeedExtra, cell);
               break;
            case 1:
               extra = mEntityFactory.CreateExtra(EntityId::BombsExtra, cell);
               break;
            case 2:
               extra = mEntityFactory.CreateExtra(EntityId::RangeExtra, cell);
               break;
         }
      }
      cell->SetExtra(extra);
   }

   arena->SetCells(cells);
   return arena;
}

std::vector<std::shared_ptr<Cell>> ArenaGenerator::CreateDefaultCells(
   const int cells_x,
   const int cells_y,
   const std::shared_ptr<Arena>& arena
) const
{
   const auto cell_size = GetCellSize(cells_x, cells_y);
   std::vector<std::shared_ptr<Cell>> cells(cells_x * cells_y);

   for (int i = 0; i < (cells_x * cells_y); i++)
   {
      const int cell_field_pos_x = i % cells_x;
      const int cell_field_pos_y = i / cells_x;

      auto cell = mEntityFactory.CreateCell(arena,
                                            cell_field_pos_x,
                                            cell_field_pos_y);
      cell->SetPosition({ arena->GetPosition().X + mBorders.Width + (cell_size.Width * cell_field_pos_x),
                          arena->GetPosition().Y + mBorders.Height + (cell_size.Height * cell_field_pos_y) });
      cell->SetSize(cell_size);

      // Create the field boundary and pattern.
      if ((cell_field_pos_x % 2) && (cell_field_pos_y % 2)) // Pattern
      {
         auto wall = mEntityFactory.CreateWall(EntityId::IndestructibleWall, cell);
         cell->SetWall(wall);
      }
      else
      {
         auto wall = mEntityFactory.CreateWall(EntityId::DestructibleWall, cell);
         cell->SetWall(wall);
      }
      cells[i] = cell;
   }
   return cells;
}

Size ArenaGenerator::GetCellSize(const int cells_x, const int cells_y) const
{
   return { (mSize.Width - (mBorders.Width * 2)) / cells_x,
            (mSize.Height - (mBorders.Height * 2)) / cells_y };
}

bool ArenaGenerator::ShouldCreateItem() const
{
   // 25% chance to generate '0' and return true.
   return !(rand() % 4);
}
