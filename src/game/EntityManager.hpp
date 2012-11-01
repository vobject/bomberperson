#ifndef ENTITYMANAGER_HPP
#define ENTITYMANAGER_HPP

#include "EventListener.hpp"

#include <memory>
#include <set>

class BomberPersonConfig;
class EventQueue;

class CreateArenaEvent;
class CreateScoreboardEvent;
class CreateWallEvent;
class CreateExtraEvent;
class CreateBombEvent;
class CreateExplosionEvent;
class CreatePlayerEvent;

class RemoveArenaEvent;
class RemoveScoreboardEvent;
class RemoveWallEvent;
class RemoveExtraEvent;
class RemoveBombEvent;
class RemoveExplosionEvent;
class RemovePlayerEvent;

class SceneObject;
class MainMenu;
class MenuItem;
class MenuItemSelector;
class Arena;

enum class UiItemId;

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

class EntityManager : public EventListener
{
public:
   EntityManager(BomberPersonConfig& cfg, EventQueue& queue);
   ~EntityManager();

   void OnEvent(const Event& event);

   std::shared_ptr<MainMenu> CreateMainmenu();
   std::shared_ptr<MenuItem> CreateMenuItem(UiItemId id);
   std::shared_ptr<MenuItemSelector> CreateMenuItemSelector();

   EntitySet GetEntities() const;

   void Cleanup();
   void Reset();

private:
   void OnCreateArena(const CreateArenaEvent& event);
   void OnCreateScoreboard(const CreateScoreboardEvent& event);
   void OnCreateWall(const CreateWallEvent& event);
   void OnCreateExtra(const CreateExtraEvent& event);
   void OnCreateBomb(const CreateBombEvent& event);
   void OnCreateExplosion(const CreateExplosionEvent& event);
   void OnCreatePlayer(const CreatePlayerEvent& event);

   void OnRemoveArena(const RemoveArenaEvent& event);
   void OnRemoveScoreboard(const RemoveScoreboardEvent& event);
   void OnRemoveWall(const RemoveWallEvent& event);
   void OnRemoveExtra(const RemoveExtraEvent& event);
   void OnRemoveBomb(const RemoveBombEvent& event);
   void OnRemoveExplosion(const RemoveExplosionEvent& event);
   void OnRemovePlayer(const RemovePlayerEvent& event);

   BomberPersonConfig& mConfig;
   EventQueue& mEventQueue;
   EntitySet mEntities;

   // A valid arena object must be created before any other ArenaObject
   //  derivatives (wall, extra, explosion, player) are created because
   //  they will use the reference to arena object.
   std::shared_ptr<Arena> mArena;
};

#endif // ENTITYMANAGER_HPP
