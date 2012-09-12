#ifndef ARENA_GENERATOR_HPP
#define ARENA_GENERATOR_HPP

#include "../utils/Utils.hpp"

#include <memory>
#include <string>
#include <tuple>
#include <vector>

class Arena;

class EntityManager;

class ArenaGenerator
{
public:
   // TODO: Init with ResCache or Options class.
   ArenaGenerator(EntityManager& entity_factory);
   ~ArenaGenerator();

   ArenaGenerator(const ArenaGenerator&) = delete;
   ArenaGenerator& operator=(const ArenaGenerator&) = delete;

   void SetArenaPosition(const Point& pos);
   void SetArenaSize(const Size& size);
   void SetArenaBorderSize(const Size& size);

//   std::shared_ptr<Field> FromLevel(const std::string& name) const;
   std::shared_ptr<Arena> GetDefaultArena(int cells_x, int cells_y) const;
   void CreateDefaultWalls(int cells_x, int cells_y, Arena& arena) const;
   void CreateDefaultExtras(int cells_x, int cells_y, Arena& arena) const;
   void CreateDefaultSpawnAreas(int cells_x, int cells_y, int players, Arena& arena) const;

private:

   bool ShouldCreateItem() const;

   EntityManager& mEntityFactory;

   // Absolute position on the screen.
   Point mPos = { 0, 0 };
   // Size of the whole arena.
   Size mSize = { 0, 0 };
   Size mBorders = { 0, 0 };
};

#endif // ARENA_GENERATOR_HPP
