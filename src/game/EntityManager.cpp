#include "EntityManager.hpp"
#include "../BomberPersonConfig.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"

#include "MainMenu.hpp"
#include "PrepareGameMenu.hpp"
#include "MenuItem.hpp"
#include "Arena.hpp"
#include "ArenaGenerator.hpp"
#include "Scoreboard.hpp"
#include "Wall.hpp"
#include "Extra.hpp"
#include "Bomb.hpp"
#include "Explosion.hpp"
#include "Player.hpp"

EntityManager::EntityManager(BomberPersonConfig &cfg, EventQueue& queue)
   : mConfig(cfg)
   , mEventQueue(queue)
{
   mEventQueue.Register(this);
}

EntityManager::~EntityManager()
{
   mEventQueue.UnRegister(this);
}

void EntityManager::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::CreateMainMenu:
         OnCreateMainMenu(dynamic_cast<const CreateMainMenuEvent&>(event));
         break;
      case EventType::CreateSetupMenu:
         OnCreateSetupMenu(dynamic_cast<const CreateSetupMenuEvent&>(event));
         break;
      case EventType::CreateMenuItem:
         OnCreateMenuItem(dynamic_cast<const CreateMenuItemEvent&>(event));
         break;
      case EventType::CreateArena:
         OnCreateArena(dynamic_cast<const CreateArenaEvent&>(event));
         break;
      case EventType::CreateScoreboard:
         OnCreateScoreboard(dynamic_cast<const CreateScoreboardEvent&>(event));
         break;
      case EventType::CreateWall:
         OnCreateWall(dynamic_cast<const CreateWallEvent&>(event));
         break;
      case EventType::CreateExtra:
         OnCreateExtra(dynamic_cast<const CreateExtraEvent&>(event));
         break;
      case EventType::CreateBomb:
         OnCreateBomb(dynamic_cast<const CreateBombEvent&>(event));
         break;
      case EventType::CreateExplosion:
         OnCreateExplosion(dynamic_cast<const CreateExplosionEvent&>(event));
         break;
      case EventType::CreatePlayer:
         OnCreatePlayer(dynamic_cast<const CreatePlayerEvent&>(event));
         break;

      case EventType::RemoveMainMenu:
         OnRemoveMainMenu(dynamic_cast<const RemoveMainMenuEvent&>(event));
         break;
      case EventType::RemoveSetupMenu:
         OnRemoveSetupMenu(dynamic_cast<const RemoveSetupMenuEvent&>(event));
         break;
      case EventType::RemoveMenuItem:
         OnRemoveMenuItem(dynamic_cast<const RemoveMenuItemEvent&>(event));
         break;
      case EventType::RemoveArena:
         OnRemoveArena(dynamic_cast<const RemoveArenaEvent&>(event));
         break;
      case EventType::RemoveScoreboard:
         OnRemoveScoreboard(dynamic_cast<const RemoveScoreboardEvent&>(event));
         break;
      case EventType::RemoveWall:
         OnRemoveWall(dynamic_cast<const RemoveWallEvent&>(event));
         break;
      case EventType::RemoveExtra:
         OnRemoveExtra(dynamic_cast<const RemoveExtraEvent&>(event));
         break;
      case EventType::RemoveBomb:
         OnRemoveBomb(dynamic_cast<const RemoveBombEvent&>(event));
         break;
      case EventType::RemoveExplosion:
         OnRemoveExplosion(dynamic_cast<const RemoveExplosionEvent&>(event));
         break;
      case EventType::RemovePlayer:
         OnRemovePlayer(dynamic_cast<const RemovePlayerEvent&>(event));
         break;

      default:
         break;
   }
}

void EntityManager::OnCreateMainMenu(const CreateMainMenuEvent& event)
{
   auto menu = std::make_shared<MainMenu>(mEventQueue);
   menu->SetPosition(event.GetPosition());
   menu->SetSize(event.GetSize());

   mEntities.insert(menu);
}

void EntityManager::OnCreateSetupMenu(const CreateSetupMenuEvent& event)
{
   auto menu = std::make_shared<PrepareGameMenu>(mEventQueue);
   menu->SetPosition(event.GetPosition());
   menu->SetSize(event.GetSize());

   mEntities.insert(menu);
}

void EntityManager::OnCreateMenuItem(const CreateMenuItemEvent& event)
{
   auto item = std::make_shared<MenuItem>(event.GetItem(), mEventQueue);
   item->SetPosition(event.GetPosition());
   item->SetSize(event.GetSize());
   item->SetText(event.GetText());
   item->SetEnabled(event.IsEnabled());

   mEntities.insert(item);
}

void EntityManager::OnCreateArena(const CreateArenaEvent& event)
{
   // The arena object will be used internally to create every other
   //  ArenaObject derived class.
   mArena = std::make_shared<Arena>(event.GetArena(),
                                    event.GetPosition(),
                                    event.GetSize(),
                                    event.GetBorderSize(),
                                    event.GetCellsX(),
                                    event.GetCellsY(),
                                    mEventQueue);

   ArenaGenerator arena_gen(mArena->GetInstanceId(), mEventQueue);
   arena_gen.SetDimensions(event.GetCellsX(), event.GetCellsY());
   arena_gen.SetPlayers(event.GetPlayerCount());

   arena_gen.CreateDefaultWalls();
   arena_gen.CreateDefaultExtras();

   mEntities.insert(mArena);
}

void EntityManager::OnCreateScoreboard(const CreateScoreboardEvent& event)
{
   auto scoreboard = std::make_shared<Scoreboard>(mEventQueue);
   scoreboard->SetPosition(event.GetPosition());
   scoreboard->SetSize(event.GetSize());

   mEntities.insert(scoreboard);
}

void EntityManager::OnCreateWall(const CreateWallEvent& event)
{
   auto wall = std::make_shared<Wall>(mArena, event.GetWall(), mEventQueue);
   mArena->SetObjectPosition(*wall, event.GetCell());
   mArena->SetObjectSize(*wall);

   mEntities.insert(wall);
}

void EntityManager::OnCreateExtra(const CreateExtraEvent& event)
{
   auto extra = std::make_shared<Extra>(mArena, event.GetExtra(), mEventQueue);
   mArena->SetObjectPosition(*extra, event.GetCell());
   mArena->SetObjectSize(*extra);

   mEntities.insert(extra);
}

void EntityManager::OnCreateBomb(const CreateBombEvent& event)
{
   auto bomb = std::make_shared<Bomb>(mArena,
                                      event.GetBomb(),
                                      mConfig.GetBombLifetime(),
                                      event.GetOwner(),
                                      mEventQueue);
   bomb->SetRange(event.GetRange());

   mArena->SetObjectPosition(*bomb, event.GetCell());
   mArena->SetObjectSize(*bomb);

   mEntities.insert(bomb);
}

void EntityManager::OnCreateExplosion(const CreateExplosionEvent& event)
{
   auto explosion = std::make_shared<Explosion>(mArena,
                                                event.GetExplosionType(),
                                                mConfig.GetExplosionLifetime(),
                                                event.GetOwner(),
                                                mEventQueue);

   mArena->SetObjectPosition(*explosion, event.GetCell());
   mArena->SetObjectSize(*explosion);

   mEntities.insert(explosion);
}

void EntityManager::OnCreatePlayer(const CreatePlayerEvent& event)
{
   std::shared_ptr<Player> player = std::make_shared<Player>(mArena,
                                                             event.GetPlayer(),
                                                             mConfig.GetPlayerSpawnLength(),
                                                             mConfig.GetPlayerDeathLength(),
                                                             mEventQueue);
   Point spawn_cell = { -1, -1 };

   switch (event.GetPlayer())
   {
      case PlayerType::Player_1:
         spawn_cell = mConfig.GetSpawningCells()[0];
         break;
      case PlayerType::Player_2:
         spawn_cell = mConfig.GetSpawningCells()[1];
         break;
      case PlayerType::Player_3:
         spawn_cell = mConfig.GetSpawningCells()[2];
         break;
      case PlayerType::Player_4:
         spawn_cell = mConfig.GetSpawningCells()[3];
         break;
      default:
         throw "Trying to create an unknown player";
         break;
   }

   const auto parent_cell = mArena->GetCellFromCoordinates(spawn_cell.X, spawn_cell.Y);
   mArena->SetObjectPosition(*player, parent_cell);
   mArena->SetObjectSize(*player);

   mEntities.insert(player);
}

void EntityManager::OnRemoveMainMenu(const RemoveMainMenuEvent& event)
{
   (void) event;
}

void EntityManager::OnRemoveSetupMenu(const RemoveSetupMenuEvent& event)
{
   (void) event;
}

void EntityManager::OnRemoveMenuItem(const RemoveMenuItemEvent& event)
{
   (void) event;
}

void EntityManager::OnRemoveArena(const RemoveArenaEvent& event)
{
   (void) event;
}

void EntityManager::OnRemoveScoreboard(const RemoveScoreboardEvent& event)
{
   (void) event;
}

void EntityManager::OnRemoveWall(const RemoveWallEvent& event)
{
   (void) event;
}

void EntityManager::OnRemoveExtra(const RemoveExtraEvent& event)
{
   (void) event;
}

void EntityManager::OnRemoveBomb(const RemoveBombEvent& event)
{
   (void) event;
}

void EntityManager::OnRemoveExplosion(const RemoveExplosionEvent& event)
{
   (void) event;
}

void EntityManager::OnRemovePlayer(const RemovePlayerEvent& event)
{
   (void) event;
}

EntitySet EntityManager::GetEntities() const
{
   return mEntities;
}

void EntityManager::Cleanup()
{
   auto entity = std::begin(mEntities);
   while (entity != std::end(mEntities))
   {
      if ((*entity)->IsValid())
      {
         entity++;
      }
      else
      {
         entity = mEntities.erase(entity);
      }
   }
}

void EntityManager::Reset()
{
   mEntities.clear();
   mArena = nullptr;
}
