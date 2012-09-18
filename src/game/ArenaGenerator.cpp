#include "ArenaGenerator.hpp"
#include "EntityManager.hpp"
#include "Arena.hpp"
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
   const int cells_y
) const
{
   if (!mSize || !cells_x || !cells_y) {
      throw "Arena size or dimensions not valid.";
   }

   return std::make_shared<Arena>(mPos,
                                  mSize,
                                  mBorders,
                                  cells_x,
                                  cells_y);
}

void ArenaGenerator::CreateDefaultWalls(
   const int cells_x,
   const int cells_y,
   Arena& arena
) const
{
   for (int x = 0; x < cells_x; x++)
   {
      for (int y = 0; y < cells_y; y++)
      {
         const Cell cell = { x, y };
         std::shared_ptr<Wall> wall;

         if ((x % 2) && (y % 2)) // Pattern
         {
            // Create the field boundary and pattern.
            wall = mEntityFactory.CreateWall(cell, WallType::Indestructible);
         }
         else
         {
            wall = mEntityFactory.CreateWall(cell, WallType::Destructible);
         }
         arena.SetWall(cell, wall);
      }
   }
}

void ArenaGenerator::CreateDefaultExtras(
   const int cells_x,
   const int cells_y,
   Arena& arena
) const
{
   for (int x = 0; x < cells_x; x++)
   {
      for (int y = 0; y < cells_y; y++)
      {
         const Cell cell = { x, y };

         if (!arena.HasWall(cell)) {
            // Do not put extras on a plain cell without a wall.
            continue;
         }

         if (!arena.GetWall(cell)->IsValid()) {
            // Do not put extras on a cell whose wall is no longer alive.
            continue;
         }

         if (!arena.GetWall(cell)->IsDestructible()) {
            // Do not put extras on cells with an indestructible wall.
            continue;
         }

         if (!ShouldCreateItem()) {
            // Do not put extras on the cell if Fortuna is not with us.
            continue;
         }

         std::shared_ptr<Extra> extra;

         if (!(rand() % (cells_x * cells_y / 4)))
         {
            // Very little chance for a golden flame (infinite explosion range).
            extra = mEntityFactory.CreateExtra(cell, ExtraType::InfiniteRange);
         }
         else
         {
            switch (rand() % 5)
            {
               case 0:
                  extra = mEntityFactory.CreateExtra(cell, ExtraType::Speed);
                  break;
               case 1:
                  extra = mEntityFactory.CreateExtra(cell, ExtraType::Bombs);
                  break;
               case 2:
                  extra = mEntityFactory.CreateExtra(cell, ExtraType::Range);
                  break;
               case 3:
                  extra = mEntityFactory.CreateExtra(cell, ExtraType::Kick);
                  break;
               case 4:
                  extra = mEntityFactory.CreateExtra(cell, ExtraType::RemoteBombs);
                  break;
            }
         }
         arena.SetExtra(cell, extra);
      }
   }
}

void ArenaGenerator::CreateDefaultSpawnAreas(
      const int cells_x,
      const int cells_y,
      const int players,
      Arena& arena
) const
{
   // TODO: invalidate all extras on the spawn cells.

   if (players >= 1)
   {
      arena.GetWall({ 0, 0 })->Invalidate();
      arena.GetWall({ 0, 1 })->Invalidate();
      arena.GetWall({ 1, 0 })->Invalidate();

      if (arena.HasExtra({ 0, 0 })) {
          arena.GetExtra({ 0, 0 })->Invalidate();
      }
      if (arena.HasExtra({ 0, 1 })) {
          arena.GetExtra({ 0, 1 })->Invalidate();
      }
      if (arena.HasExtra({ 1, 0 })) {
          arena.GetExtra({ 1, 0 })->Invalidate();
      }
   }

   if (players >= 2)
   {
      arena.GetWall({ cells_x - 1, cells_y - 1 })->Invalidate();
      arena.GetWall({ cells_x - 1, cells_y - 2 })->Invalidate();
      arena.GetWall({ cells_x - 2, cells_y - 1 })->Invalidate();

      if (arena.HasExtra({ cells_x - 1, cells_y - 1 })) {
          arena.GetExtra({ cells_x - 1, cells_y - 1 })->Invalidate();
      }
      if (arena.HasExtra({ cells_x - 1, cells_y - 2 })) {
          arena.GetExtra({ cells_x - 1, cells_y - 2 })->Invalidate();
      }
      if (arena.HasExtra({ cells_x - 2, cells_y - 1 })) {
          arena.GetExtra({ cells_x - 2, cells_y - 1 })->Invalidate();
      }
   }

   if (players >= 3)
   {
      arena.GetWall({ 0, cells_y - 2 })->Invalidate();
      arena.GetWall({ 0, cells_y - 1 })->Invalidate();
      arena.GetWall({ 1, cells_y - 1 })->Invalidate();

      if (arena.HasExtra({ 0, cells_y - 2 })) {
          arena.GetExtra({ 0, cells_y - 2 })->Invalidate();
      }
      if (arena.HasExtra({ 0, cells_y - 1 })) {
          arena.GetExtra({ 0, cells_y - 1 })->Invalidate();
      }
      if (arena.HasExtra({ 1, cells_y - 1 })) {
          arena.GetExtra({ 1, cells_y - 1 })->Invalidate();
      }
   }

   if (players >= 4)
   {
      arena.GetWall({ cells_x - 2, 0 })->Invalidate();
      arena.GetWall({ cells_x - 1, 0 })->Invalidate();
      arena.GetWall({ cells_x - 1, 1 })->Invalidate();

      if (arena.HasExtra({ cells_x - 2, 0 })) {
          arena.GetExtra({ cells_x - 2, 0 })->Invalidate();
      }
      if (arena.HasExtra({ cells_x - 1, 0 })) {
          arena.GetExtra({ cells_x - 1, 0 })->Invalidate();
      }
      if (arena.HasExtra({ cells_x - 1, 1 })) {
          arena.GetExtra({ cells_x - 1, 1 })->Invalidate();
      }
   }
}

bool ArenaGenerator::ShouldCreateItem() const
{
   // 33% chance to generate '0' and return true.
   return !(rand() % 3);
}
