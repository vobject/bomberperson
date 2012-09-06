#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <memory>
#include <vector>

class SceneObject;
class Arena;
class Bomb;
class Cell;
class Explosion;
class InputDevice;
class Player;

class EntityManager
{
public:
   EntityManager();
   ~EntityManager();

//   std::shared_ptr<Arena> CreateArena();
//   std::shared_ptr<Cell> CreateCell();
//   std::shared_ptr<Extra> CreateExtra();
   std::shared_ptr<Bomb> CreateBomb(const std::shared_ptr<Cell>& cell);
   std::shared_ptr<Explosion> CreateExplosion(const std::shared_ptr<Cell>& cell);
   void CreatePlayers(int count, const std::shared_ptr<Arena>& arena);

   std::vector<std::shared_ptr<Player>> GetPlayers() const;
   std::vector<std::shared_ptr<SceneObject>> GetEntities() const;

   void Reset();

private:
   std::vector<std::shared_ptr<Bomb>> mBombs;
   std::vector<std::shared_ptr<Explosion>> mExplosions;
   std::vector<std::shared_ptr<Player>> mPlayers;
   std::vector<std::shared_ptr<SceneObject>> mEntities;
};

#endif // ENTITYMANAGER_HPP
