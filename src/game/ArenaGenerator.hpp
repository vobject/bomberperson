#ifndef ARENA_GENERATOR_HPP
#define ARENA_GENERATOR_HPP

#include "../utils/Utils.hpp"

#include <memory>
#include <string>
#include <tuple>
#include <vector>

class Arena;
class Cell;
enum class CellItem;

class ArenaGenerator
{
public:
   ArenaGenerator(); // TODO: Init with ResCache or Options class.
   ~ArenaGenerator();

   ArenaGenerator(const ArenaGenerator&) = delete;
   ArenaGenerator& operator=(const ArenaGenerator&) = delete;

   void SetArenaPosition(const Point& pos);
   void SetArenaSize(const Size& size);

//   std::shared_ptr<Field> FromLevel(const std::string& name) const;
   std::shared_ptr<Arena> GetDefaultArena(int cells_x, int cells_y, int players) const;

private:
   std::vector<std::shared_ptr<Cell>> CreateDefaultCells(int cells_x,
         int cells_y, const std::shared_ptr<Arena>& arena) const;

   Size GetCellSize(int cells_x, int cells_y) const;
   bool ShouldCreateItem() const;

   // Absolute position on the screen.
   Point mPos = { 0, 0 };
   // Size of the playing field.
   Size mSize = { 0, 0 };
};

#endif // ARENA_GENERATOR_HPP
