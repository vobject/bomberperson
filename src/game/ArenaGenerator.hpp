#ifndef ARENA_GENERATOR_HPP
#define ARENA_GENERATOR_HPP

#include "../utils/Utils.hpp"

#include <memory>

class EventQueue;
class Cell;

class ArenaGenerator
{
public:
   ArenaGenerator(unsigned int id, EventQueue& queue);
   ~ArenaGenerator();

   ArenaGenerator(const ArenaGenerator&) = delete;
   ArenaGenerator& operator=(const ArenaGenerator&) = delete;

   void SetDimensions(int cells_x, int cells_y);
   void SetPlayers(int player_count);

   void CreateDefaultWalls() const;
   void CreateDefaultExtras() const;

private:
   void CreateDefaultWall(const Cell& cell) const;
   void CreateDefaultExtra(const Cell& cell) const;

   bool IsPatternCell(const Cell& cell) const;
   bool IsSpawningCell(const Cell& cell) const;
   bool ShouldCreateItem() const;

   const unsigned int mArenaId;

   EventQueue& mEventQueue;

   int mCellsX = 0;
   int mCellsY = 0;
   int mPlayers = 0;
};

#endif // ARENA_GENERATOR_HPP
