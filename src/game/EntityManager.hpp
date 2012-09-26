#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include <memory>
#include <set>

class EventQueue;

class SceneObject;
class MainMenu;
class MenuItem;
class MenuItemSelector;
class Arena;
class Scoreboard;
class Wall;
class Extra;
class Bomb;
class Explosion;
class Player;
struct Cell;

enum class EntityId;
enum class UiItemId;

enum class WallType;
enum class ExtraType;
enum class BombType;
enum class ExplosionType;
enum class PlayerType;

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
   EntityManager(EventQueue& queue);
   ~EntityManager();

   std::shared_ptr<MainMenu> CreateMainmenu();
   std::shared_ptr<MenuItem> CreateMenuItem(UiItemId id);
   std::shared_ptr<MenuItemSelector> CreateMenuItemSelector();

   // A valid arena object should be created before any other ArenaObject
   //  derivatives (wall, extra, explosion, player) are created because
   //  they will use the reference to arena object.
   std::shared_ptr<Arena> CreateArena(int player_count);
   std::shared_ptr<Scoreboard> CreateScoreboard();

   std::shared_ptr<Wall> CreateWall(const Cell& cell, WallType type);
   std::shared_ptr<Extra> CreateExtra(const Cell& cell, ExtraType type);
   std::shared_ptr<Bomb> CreateBomb(const Cell& cell, BombType type, PlayerType owner);
   std::shared_ptr<Explosion> CreateExplosion(const Cell& cell, ExplosionType type, const std::shared_ptr<Player>& owner);
   std::shared_ptr<Player> CreatePlayer(PlayerType type);

   EntitySet GetEntities() const;

   void Cleanup();
   void Reset();

private:
   EventQueue& mEventQueue;
   std::shared_ptr<Arena> mArena;
   EntitySet mEntities;
};

#endif // ENTITYMANAGER_HPP
