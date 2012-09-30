#include "EntityManager.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"

#include "MainMenu.hpp"
#include "MenuItem.hpp"
#include "MenuItemSelector.hpp"
#include "Arena.hpp"
#include "ArenaGenerator.hpp"
#include "Scoreboard.hpp"
#include "Wall.hpp"
#include "Extra.hpp"
#include "Bomb.hpp"
#include "Explosion.hpp"
#include "Player.hpp"
#include "../render/Renderer.hpp"
#include "../Options.hpp"

EntityManager::EntityManager(EventQueue& queue, const std::shared_ptr<Renderer>& renderer)
   : mEventQueue(queue)
   , mRenderer(renderer)
{
   mEventQueue.Register(this);
}

EntityManager::~EntityManager()
{
   mEventQueue.UnRegister(this);
   mRenderer->Clear();
}

void EntityManager::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
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

std::shared_ptr<MainMenu> EntityManager::CreateMainmenu()
{
   auto menu = std::make_shared<MainMenu>(*this);

   mEntities.insert(menu);
   return menu;
}

std::shared_ptr<MenuItem> EntityManager::CreateMenuItem(const UiItemId id)
{
   auto item = std::make_shared<MenuItem>(id);

   mEntities.insert(item);
   return item;
}

std::shared_ptr<MenuItemSelector> EntityManager::CreateMenuItemSelector()
{
   auto selector = std::make_shared<MenuItemSelector>();
   selector->SetSize({ 48, 48 });

   mEntities.insert(selector);
   return selector;
}

void EntityManager::OnCreateArena(const CreateArenaEvent& event)
{
   // The arena object will be used internally to create every other
   //  ArenaObject derived class.
   mArena = std::make_shared<Arena>(event.GetPosition(),
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
   mRenderer->AddObject(mArena);
}

void EntityManager::OnCreateScoreboard(const CreateScoreboardEvent& event)
{
   auto scoreboard = std::make_shared<Scoreboard>(mEventQueue);
   scoreboard->SetPosition(event.GetPosition());
   scoreboard->SetSize(event.GetSize());

   mEntities.insert(scoreboard);
//   mRenderer->AddObject(scoreboard);
}

void EntityManager::OnCreateWall(const CreateWallEvent& event)
{
   auto wall = std::make_shared<Wall>(mArena, event.GetWall(), mEventQueue);
   mArena->SetObjectPosition(*wall, event.GetCell());
   mArena->SetObjectSize(*wall);

   mEntities.insert(wall);
   mRenderer->AddObject(wall);
}

void EntityManager::OnCreateExtra(const CreateExtraEvent& event)
{
//   if (mArena->HasWall(event.GetCell()) &&
//       !mArena->GetWall(event.GetCell())->IsDestructible())
//   {
//      // Do not create extras on cells with indestructible walls
//      //  because a player could never reach it.
//      return;
//   }

   auto extra = std::make_shared<Extra>(mArena, event.GetExtra(), mEventQueue);
   mArena->SetObjectPosition(*extra, event.GetCell());
   mArena->SetObjectSize(*extra);

   mEntities.insert(extra);
//   mRenderer->AddObject(extra);
}

void EntityManager::OnCreateBomb(const CreateBombEvent& event)
{
   auto bomb = std::make_shared<Bomb>(mArena,
                                      event.GetBomb(),
                                      event.GetOwner(),
                                      mEventQueue);
   bomb->SetRange(event.GetRange());

   mArena->SetObjectPosition(*bomb, event.GetCell());
   mArena->SetObjectSize(*bomb);

   mEntities.insert(bomb);
//   mRenderer->AddObject(bomb);
}

void EntityManager::OnCreateExplosion(const CreateExplosionEvent& event)
{
   auto explosion = std::make_shared<Explosion>(mArena,
                                                event.GetExplosionType(),
                                                event.GetOwner(),
                                                mEventQueue);

   mArena->SetObjectPosition(*explosion, event.GetCell());
   mArena->SetObjectSize(*explosion);

   mEntities.insert(explosion);
//   mRenderer->AddObject(explosion);
}

void EntityManager::OnCreatePlayer(const CreatePlayerEvent& event)
{
   std::shared_ptr<Player> player = std::make_shared<Player>(mArena,
                                                             event.GetPlayer(),
                                                             mEventQueue);
   Cell parent_cell = { -1, -1 };

   switch (event.GetPlayer())
   {
      case PlayerType::Player_1:
         parent_cell = mArena->GetCellFromCoordinates(DefaultValue::PLAYER_1_CELL_X, DefaultValue::PLAYER_1_CELL_Y);
         break;
      case PlayerType::Player_2:
         parent_cell = mArena->GetCellFromCoordinates(DefaultValue::PLAYER_2_CELL_X, DefaultValue::PLAYER_2_CELL_Y);
         break;
      case PlayerType::Player_3:
         parent_cell = mArena->GetCellFromCoordinates(DefaultValue::PLAYER_3_CELL_X, DefaultValue::PLAYER_3_CELL_Y);
         break;
      case PlayerType::Player_4:
         parent_cell = mArena->GetCellFromCoordinates(DefaultValue::PLAYER_4_CELL_X, DefaultValue::PLAYER_4_CELL_Y);
         break;
      default:
         throw "Trying to create an unknown player";
         break;
   }
   mArena->SetObjectPosition(*player, parent_cell);
   mArena->SetObjectSize(*player);

   mEntities.insert(player);
//   mRenderer->AddObject(player);
}

void EntityManager::OnRemoveArena(const RemoveArenaEvent& event)
{
   mRenderer->RemoveObject(event.GetSender());
}

void EntityManager::OnRemoveScoreboard(const RemoveScoreboardEvent& event)
{
//   mRenderer->RemoveObject(event.GetSender());
}

void EntityManager::OnRemoveWall(const RemoveWallEvent& event)
{
   mRenderer->RemoveObject(event.GetSender());
}

void EntityManager::OnRemoveExtra(const RemoveExtraEvent& event)
{
//   mRenderer->RemoveObject(event.GetSender());
}

void EntityManager::OnRemoveBomb(const RemoveBombEvent& event)
{
//   mRenderer->RemoveObject(event.GetSender());
}

void EntityManager::OnRemoveExplosion(const RemoveExplosionEvent& event)
{
//   mRenderer->RemoveObject(event.GetSender());
}

void EntityManager::OnRemovePlayer(const RemovePlayerEvent& event)
{
//   mRenderer->RemoveObject(event.GetSender());
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
