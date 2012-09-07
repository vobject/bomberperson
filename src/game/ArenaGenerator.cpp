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

   if (players >= 1)
   {
      cells[0]->SetWall(nullptr);
      cells[1]->SetWall(nullptr);
      cells[cells_x]->SetWall(nullptr);
   }

   if (players >= 2)
   {
      cells[cells_x * cells_y - 1]->SetWall(nullptr);
      cells[cells_x * cells_y - 2]->SetWall(nullptr);
      cells[cells_x * cells_y - cells_x - 1]->SetWall(nullptr);
   }

   if (players >= 3)
   {
      cells[cells_x * cells_y - cells_x * 2]->SetWall(nullptr);
      cells[cells_x * cells_y - cells_x]->SetWall(nullptr);
      cells[cells_x * cells_y - cells_x + 1]->SetWall(nullptr);
   }

   if (players >= 4)
   {
      cells[cells_x - 2]->SetWall(nullptr);
      cells[cells_x - 1]->SetWall(nullptr);
      cells[cells_x * 2 - 1]->SetWall(nullptr);
   }

   for (auto& cell : cells)
   {
      if (cell->HasWall() && !cell->GetWall()->IsDestructible()) {
         continue;
      }

      if (!cell->HasWall()) {
         continue;
      }

      if (!ShouldCreateItem()) {
         continue;
      }

      std::shared_ptr<Extra> extra;

      if (!(rand() % (cells_x * cells_y / 3)))
      {
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

      auto cell = mEntityFactory.CreateCell(cell_field_pos_x,
                                            cell_field_pos_y,
                                            arena);
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
