#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <memory>
#include <vector>

class SceneObject;
class Arena;
class Extra;
class Bomb;
class Cell;
class Wall;
class Explosion;
class InputDevice;
class Player;
enum class EntityId;

class EntityManager
{
public:
   EntityManager();
   ~EntityManager();

   std::shared_ptr<Arena> CreateArena(int player_count);
   std::shared_ptr<Cell> CreateCell(int x, int y, const std::shared_ptr<Arena>& arena);
   std::shared_ptr<Wall> CreateWall(EntityId id, const std::shared_ptr<Cell>& cell);
   std::shared_ptr<Extra> CreateExtra(EntityId id, const std::shared_ptr<Cell>& cell);
   std::shared_ptr<Bomb> CreateBomb(const std::shared_ptr<Cell>& cell);
   std::shared_ptr<Explosion> CreateExplosion(const std::shared_ptr<Cell>& cell);

   std::shared_ptr<Player> CreatePlayer(EntityId id, const std::shared_ptr<Arena>& arena);

//   std::shared_ptr<Arena> GetArena() const;
//   std::vector<std::shared_ptr<Player>> GetPlayers() const;
   std::vector<std::shared_ptr<SceneObject>> GetEntities() const;

   void Reset();

private:
//   std::shared_ptr<Arena> mArena;
//   std::vector<std::shared_ptr<Extra>> mExtras;
//   std::vector<std::shared_ptr<Bomb>> mBombs;
//   std::vector<std::shared_ptr<Explosion>> mExplosions;
//   std::vector<std::shared_ptr<Player>> mPlayers;
   std::vector<std::shared_ptr<SceneObject>> mEntities;
};

#endif // ENTITYMANAGER_HPP
