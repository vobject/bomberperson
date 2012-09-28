#include "ArenaGenerator.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"

#include "Arena.hpp"
#include "Wall.hpp"
#include "Extra.hpp"
#include "../Options.hpp"

#include <cstdlib>
#include <ctime>

ArenaGenerator::ArenaGenerator(unsigned int id, EventQueue& queue)
   : mArenaId(id)
   , mEventQueue(queue)
{
   srand(time(nullptr));
}

ArenaGenerator::~ArenaGenerator()
{

}

void ArenaGenerator::SetDimensions(const int cells_x, const int cells_y)
{
   mCellsX = cells_x;
   mCellsY = cells_y;
}

void ArenaGenerator::SetPlayers(const int player_count)
{
   mPlayers = player_count;
}

void ArenaGenerator::CreateDefaultWalls() const
{
   for (int x = 0; x < mCellsX; x++)
   {
      for (int y = 0; y < mCellsY; y++)
      {
         const Cell cell = { x, y };

         // Do not plant bombs on a plyer's spawning area.
         if (IsSpawningCell(cell)) {
            continue;
         }

         CreateDefaultWall(cell);
      }
   }
}

void ArenaGenerator::CreateDefaultExtras() const
{
   for (int x = 0; x < mCellsX; x++)
   {
      for (int y = 0; y < mCellsY; y++)
      {
         const Cell cell = { x, y };

         if (IsPatternCell(cell)) {
            // Do not put extras on cells with an indestructible wall.
            continue;
         }

         if (IsSpawningCell(cell)) {
            // Do not put extras on a cell without a wall.
            continue;
         }

         if (!ShouldCreateItem()) {
            // Do not put extras on the cell if Fortuna is not with us.
            continue;
         }

         CreateDefaultExtra(cell);
      }
   }
}

void ArenaGenerator::CreateDefaultWall(const Cell& cell) const
{
   WallType type = WallType::Destructible;

   if (IsPatternCell(cell))
   {
      // Create a pattern of indestructible walls.
      type = WallType::Indestructible;
   }

   mEventQueue.Add(std::make_shared<CreateWallEvent>(mArenaId, cell, type));
}

void ArenaGenerator::CreateDefaultExtra(const Cell& cell) const
{
   ExtraType type;

   if (!(rand() % (mCellsX * mCellsY / 4)))
   {
      // Very little chance for a golden flame (infinite explosion range).
      type = ExtraType::InfiniteRange;
   }
   else
   {
      switch (rand() % 5)
      {
         case 0:
            type = ExtraType::Speed;
            break;
         case 1:
            type = ExtraType::Bombs;
            break;
         case 2:
            type = ExtraType::Range;
            break;
         case 3:
            type = ExtraType::Kick;
            break;
         case 4:
            type = ExtraType::RemoteBombs;
            break;
      }
   }

   mEventQueue.Add(std::make_shared<CreateExtraEvent>(mArenaId, cell, type));
}

bool ArenaGenerator::IsPatternCell(const Cell& cell) const
{
   return (cell.X % 2) && (cell.Y % 2);
}

bool ArenaGenerator::IsSpawningCell(const Cell& cell) const
{
   if ((mPlayers >= 1) && ((cell == Cell{ 0, 0 }) ||
                           (cell == Cell{ 0, 1 }) ||
                           (cell == Cell{ 1, 0 })))
   {
      return true;
   }

   if ((mPlayers >= 2) && ((cell == Cell{ mCellsX - 1, mCellsY - 1 }) ||
                           (cell == Cell{ mCellsX - 1, mCellsY - 2 }) ||
                           (cell == Cell{ mCellsX - 2, mCellsY - 1 })))
   {
      return true;
   }

   if ((mPlayers >= 3) && ((cell == Cell{ 0, mCellsY - 2 }) ||
                           (cell == Cell{ 0, mCellsY - 1 }) ||
                           (cell == Cell{ 1, mCellsY - 1 })))
   {
      return true;
   }

   if ((mPlayers >= 4) && ((cell == Cell{ mCellsX - 2, 0 }) ||
                           (cell == Cell{ mCellsX - 1, 0 }) ||
                           (cell == Cell{ mCellsX - 1, 1 })))
   {
      return true;
   }

   return false;
}

bool ArenaGenerator::ShouldCreateItem() const
{
   // 33% chance to generate '0' and return true.
   return !(rand() % 3);
}
