#include "EntityManager.hpp"
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
#include "../Options.hpp"

EntityManager::EntityManager(EventQueue& queue)
   : mEventQueue(queue)
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
      case EventType::CreateBomb:
         OnCreateBomb(dynamic_cast<const CreateBombEvent&>(event));
         break;
      case EventType::CreateExplosion:
         OnCreateExplosion(dynamic_cast<const CreateExplosionEvent&>(event));
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

std::shared_ptr<Arena> EntityManager::CreateArena(const int player_count)
{
   ArenaGenerator arena_gen(*this);
   arena_gen.SetArenaPosition({ DefaultValue::ARENA_POS_X, DefaultValue::ARENA_POS_Y });
   arena_gen.SetArenaSize({ DefaultValue::ARENA_WIDTH, DefaultValue::ARENA_HEIGHT });
   arena_gen.SetArenaBorderSize({ DefaultValue::ARENA_BORDER_WIDTH, DefaultValue::ARENA_BORDER_HEIGHT });

   // The arena object will be used internally to create every other
   //  ArenaObject derived class.
   mArena = arena_gen.GetDefaultArena(DefaultValue::ARENA_CELLS_X, DefaultValue::ARENA_CELLS_Y);
   arena_gen.CreateDefaultWalls(DefaultValue::ARENA_CELLS_X, DefaultValue::ARENA_CELLS_Y, *mArena);
   arena_gen.CreateDefaultExtras(DefaultValue::ARENA_CELLS_X, DefaultValue::ARENA_CELLS_Y, *mArena);
   arena_gen.CreateDefaultSpawnAreas(DefaultValue::ARENA_CELLS_X, DefaultValue::ARENA_CELLS_Y, player_count, *mArena);

   mEntities.insert(mArena);
   return mArena;
}

std::shared_ptr<Scoreboard> EntityManager::CreateScoreboard()
{
   auto scoreboard = std::make_shared<Scoreboard>();
   scoreboard->SetPosition({ DefaultValue::SCOREBOARD_POS_X, DefaultValue::SCOREBOARD_POS_Y });
   scoreboard->SetSize({ DefaultValue::SCOREBOARD_WIDTH, DefaultValue::SCOREBOARD_HEIGHT });

   mEntities.insert(scoreboard);
   return scoreboard;
}

std::shared_ptr<Wall> EntityManager::CreateWall(
   const Cell& cell,
   const WallType type
)
{
   auto wall = std::make_shared<Wall>(mArena, type);
   mArena->SetObjectPosition(*wall, cell);
   mArena->SetObjectSize(*wall);

   mEntities.insert(wall);
   return wall;
}

std::shared_ptr<Extra> EntityManager::CreateExtra(
   const Cell& cell,
   const ExtraType type
)
{
   auto extra = std::make_shared<Extra>(mArena, type);
   mArena->SetObjectPosition(*extra, cell);
   mArena->SetObjectSize(*extra);

   mEntities.insert(extra);
   return extra;
}

void EntityManager::OnCreateBomb(const CreateBombEvent& event)
{
   auto bomb = std::make_shared<Bomb>(mArena,
                                      event.GetBombType(),
                                      event.GetOwner(),
                                      mEventQueue);
   bomb->SetRange(event.GetRange());

   mArena->SetObjectPosition(*bomb, event.GetCell());
   mArena->SetObjectSize(*bomb);
   mArena->SetBomb(event.GetCell(), bomb);

   mEntities.insert(bomb);
}

void EntityManager::OnCreateExplosion(const CreateExplosionEvent& event)
{
   auto explosion = std::make_shared<Explosion>(mArena,
                                                event.GetExplosionType(),
                                                event.GetOwner());

   mArena->SetObjectPosition(*explosion, event.GetCell());
   mArena->SetObjectSize(*explosion);
   mArena->SetExplosion(event.GetCell(), explosion);

   mEntities.insert(explosion);
}

std::shared_ptr<Player> EntityManager::CreatePlayer(const PlayerType type)
{
   std::shared_ptr<Player> player = std::make_shared<Player>(mArena,
                                                             type,
                                                             mEventQueue);
   Cell parent_cell = { -1, -1 };

   switch (type)
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
   }
   mArena->SetObjectPosition(*player, parent_cell);
   player->SetSize({ DefaultValue::CELL_WIDTH, DefaultValue::CELL_HEIGHT });

   mEntities.insert(player);
   return player;
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
