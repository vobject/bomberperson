#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <memory>
#include <set>

enum class EntityId;
class SceneObject;
class Arena;
class Scoreboard;
class Cell;
class Wall;
class Extra;
class Bomb;
class Explosion;
class Player;

template<class T>
struct Sort_Dereferenced_SharedPtr
{
   bool operator()(const std::shared_ptr<T>& lhs,
                   const std::shared_ptr<T>& rhs) const
   {
      return *lhs < *rhs;
   }
};

typedef std::multiset<std::shared_ptr<SceneObject>, Sort_Dereferenced_SharedPtr<SceneObject>> EntitySet;

class EntityManager
{
public:
   EntityManager();
   ~EntityManager();

   std::shared_ptr<Arena> CreateArena(int player_count);
   std::shared_ptr<Scoreboard> CreateScoreboard();
   std::shared_ptr<Cell> CreateCell(const std::shared_ptr<Arena>& arena, int x, int y);
   std::shared_ptr<Wall> CreateWall(EntityId id, const std::shared_ptr<Cell>& cell);
   std::shared_ptr<Extra> CreateExtra(EntityId id, const std::shared_ptr<Cell>& cell);
   std::shared_ptr<Bomb> CreateBomb(const std::shared_ptr<Cell>& cell);
   std::shared_ptr<Explosion> CreateExplosion(const std::shared_ptr<Cell>& cell);
   std::shared_ptr<Player> CreatePlayer(EntityId id, const std::shared_ptr<Arena>& arena);

   EntitySet GetEntities() const;

   void Cleanup();
   void Reset();

private:
   EntitySet mEntities;
};

#endif // ENTITYMANAGER_HPP
