#include "ArenaGenerator.hpp"
#include "Arena.hpp"
#include "Cell.hpp"
#include "Wall.hpp"
#include "Extra.hpp"
#include "../Options.hpp"

#include <cstdlib>
#include <ctime>

ArenaGenerator::ArenaGenerator()
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

std::shared_ptr<Arena> ArenaGenerator::GetDefaultArena(
   const int cells_x,
   const int cells_y,
   const int players
) const
{
   if (!mSize) {
      throw "Arena size not yet set.";
   }

   auto arena = std::make_shared<Arena>(DefaultOptions::ARENA_ID);
   arena->SetPosition(mPos);
   arena->SetSize(mSize);
   arena->SetDimensions(cells_x, cells_y);

   const auto cells = CreateDefaultCells(cells_x, cells_y, arena);

   if (players >= 1) {
      cells[0 + (0 * cells_x)]->SetWall(nullptr);
      cells[0 + (1 * cells_x)]->SetWall(nullptr);
      cells[1 + (0 * cells_x)]->SetWall(nullptr);
   }
   if (players >= 2) {
      cells[(cells_x - 3) + ((cells_x - 2) * cells_x)]->SetWall(nullptr);
      cells[(cells_x - 2) + ((cells_x - 3) * cells_x)]->SetWall(nullptr);
      cells[(cells_x - 2) + ((cells_x - 2) * cells_x)]->SetWall(nullptr);
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

      std::string extra_name;
      ExtraType extra_type;

      switch (rand() % 3)
      {
         case 0:
            extra_name = DefaultOptions::EXTRA_ID_SPEED;
            extra_type = ExtraType::Speed;
            break;
         case 1:
            extra_name = DefaultOptions::EXTRA_ID_BOMBS;
            extra_type = ExtraType::BombSupply;
            break;
         case 2:
            extra_name = DefaultOptions::EXTRA_ID_RANGE;
            extra_type = ExtraType::BombRange;
            break;
      }

      auto extra = std::make_shared<Extra>(extra_name, extra_type);
      extra->SetPosition(cell->GetPosition());
      extra->SetSize(cell->GetSize());
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

      auto cell = std::make_shared<Cell>(DefaultOptions::CELL_ID,
                                         cell_field_pos_x,
                                         cell_field_pos_y,
                                         arena);

      cell->SetPosition({ arena->GetPosition().X + (cell_size.Width * cell_field_pos_x),
                          arena->GetPosition().Y + (cell_size.Height * cell_field_pos_y) });
      cell->SetSize(cell_size);

      // Create the field boundary and pattern.
      if ((cell_field_pos_x % 2) && (cell_field_pos_y % 2)) // Pattern
      {
         auto wall = std::make_shared<Wall>(DefaultOptions::WALL_ID_BRICKS,
                                            WallType::Indestructible);
         wall->SetPosition(cell->GetPosition());
         wall->SetSize(cell->GetSize());
         cell->SetWall(wall);
      }
      else
      {
         auto wall = std::make_shared<Wall>(DefaultOptions::WALL_ID_WOOD,
                                            WallType::Destructible);
         wall->SetPosition(cell->GetPosition());
         wall->SetSize(cell->GetSize());
         cell->SetWall(wall);
      }
      cells[i] = cell;
   }
   return cells;
}

Size ArenaGenerator::GetCellSize(const int cells_x, const int cells_y) const
{
   return { mSize.Width / cells_x, mSize.Height / cells_y };
}

bool ArenaGenerator::ShouldCreateItem() const
{
   // 25% chance to generate '0' and return true.
   return !(rand() % 4);
}
