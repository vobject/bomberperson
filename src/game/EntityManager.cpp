#include "EntityManager.hpp"
#include "EntityId.hpp"
#include "Arena.hpp"
#include "ArenaGenerator.hpp"
#include "Extra.hpp"
#include "Bomb.hpp"
#include "Wall.hpp"
#include "Explosion.hpp"
#include "Cell.hpp"
#include "Player.hpp"
#include "../utils/Utils.hpp"
#include "../Options.hpp"

EntityManager::EntityManager()
{
}

EntityManager::~EntityManager()
{
}

std::shared_ptr<Arena> EntityManager::CreateArena(const int player_count)
{
   ArenaGenerator arena_gen(*this);
   arena_gen.SetArenaPosition({ DefaultSize::ARENA_POS_X, DefaultSize::ARENA_POS_Y });
   arena_gen.SetArenaSize({ DefaultSize::ARENA_WIDTH, DefaultSize::ARENA_HEIGHT });
   arena_gen.SetArenaBorderSize({ DefaultSize::ARENA_BORDER_WIDTH, DefaultSize::ARENA_BORDER_HEIGHT });
   auto arena = arena_gen.GetDefaultArena(DefaultSize::ARENA_CELLS_X, DefaultSize::ARENA_CELLS_Y, player_count);

   mEntities.push_back(arena);
   return arena;
}

std::shared_ptr<Cell> EntityManager::CreateCell(
   const int x,
   const int y,
   const std::shared_ptr<Arena> &arena
)
{
   auto cell = std::make_shared<Cell>(x, y, arena);

   mEntities.push_back(cell);
   return cell;
}

std::shared_ptr<Wall> EntityManager::CreateWall(
   const EntityId id,
   const std::shared_ptr<Cell>& cell
)
{
   auto wall = std::make_shared<Wall>(id);
   wall->SetPosition(cell->GetPosition());
   wall->SetSize(cell->GetSize());

   mEntities.push_back(wall);
   return wall;
}

std::shared_ptr<Extra> EntityManager::CreateExtra(
   const EntityId id,
   const std::shared_ptr<Cell>& cell
)
{
   auto extra = std::make_shared<Extra>(id);
   extra->SetPosition(cell->GetPosition());
   extra->SetSize(cell->GetSize());

//   mExtras.push_back(extra);
   mEntities.push_back(extra);
   return extra;
}

std::shared_ptr<Bomb> EntityManager::CreateBomb(const std::shared_ptr<Cell>& cell)
{
   auto bomb = std::make_shared<Bomb>(*this, cell);
   bomb->SetSize(cell->GetSize());
   bomb->SetPosition(cell->GetPosition());

//   mBombs.push_back(bomb);
   mEntities.push_back(bomb);
   return bomb;
}

std::shared_ptr<Explosion> EntityManager::CreateExplosion(const std::shared_ptr<Cell>& cell)
{
   auto explosion = std::make_shared<Explosion>();
   explosion->SetSize(cell->GetSize());
   explosion->SetPosition(cell->GetPosition());

//   mExplosions.push_back(explosion);
   mEntities.push_back(explosion);
   return explosion;
}

std::shared_ptr<Player> EntityManager::CreatePlayer(
   const EntityId id,
   const std::shared_ptr<Arena>& arena
)
{
   std::shared_ptr<Player> player;
   std::shared_ptr<Cell> parent_cell;

   switch (id)
   {
      case EntityId::Player_1:
         player = std::make_shared<Player>(id, *this);
         parent_cell = arena->GetCellFromCoordinates(DefaultSize::PLAYER_1_CELL_X, DefaultSize::PLAYER_1_CELL_Y);
         break;
      case EntityId::Player_2:
         player = std::make_shared<Player>(id, *this);
         parent_cell = arena->GetCellFromCoordinates(DefaultSize::PLAYER_2_CELL_X, DefaultSize::PLAYER_2_CELL_Y);
         break;
      case EntityId::Player_3:
         player = std::make_shared<Player>(id, *this);
         parent_cell = arena->GetCellFromCoordinates(DefaultSize::PLAYER_3_CELL_X, DefaultSize::PLAYER_3_CELL_Y);
         break;
      case EntityId::Player_4:
         player = std::make_shared<Player>(id, *this);
         parent_cell = arena->GetCellFromCoordinates(DefaultSize::PLAYER_4_CELL_X, DefaultSize::PLAYER_4_CELL_Y);
         break;
      default:
         LOG(logERROR) << "Trying to create an unknown player.";
         return nullptr;
   }

   player->SetParentCell(parent_cell);
   player->SetPosition(parent_cell->GetPosition());
   player->SetSize({ DefaultSize::PLAYER_WIDTH, DefaultSize::PLAYER_HEIGHT });

//   mPlayers.push_back(player);
   mEntities.push_back(player);
   return player;
}

//void EntityManager::CreatePlayers(
//   const int count,
//   const std::shared_ptr<Arena>& arena
//)
//{
//   for (int i = 0; i < count; i++)
//   {
//      std::shared_ptr<Player> player = nullptr;
//      std::shared_ptr<Cell> parent_cell = nullptr;

//      switch (i)
//      {
//         case 0:
//            player = std::make_shared<Player>(EntityId::Player_1, *this);
//            parent_cell = arena->GetCellFromCoordinates(DefaultSize::PLAYER_1_CELL_X, DefaultSize::PLAYER_1_CELL_Y);
//            break;
//         case 1:
//            player = std::make_shared<Player>(EntityId::Player_2, *this);
//            parent_cell = arena->GetCellFromCoordinates(DefaultSize::PLAYER_2_CELL_X, DefaultSize::PLAYER_2_CELL_Y);
//            break;
//         case 2:
//            player = std::make_shared<Player>(EntityId::Player_3, *this);
//            parent_cell = arena->GetCellFromCoordinates(DefaultSize::PLAYER_3_CELL_X, DefaultSize::PLAYER_3_CELL_Y);
//            break;
//         case 3:
//            player = std::make_shared<Player>(EntityId::Player_4, *this);
//            parent_cell = arena->GetCellFromCoordinates(DefaultSize::PLAYER_4_CELL_X, DefaultSize::PLAYER_4_CELL_Y);
//            break;
//         default:
//            LOG(logERROR) << "Trying to create more players than supported.";
//            return;
//      }

//      player->SetParentCell(parent_cell);
//      player->SetPosition(parent_cell->GetPosition());
//      player->SetSize({ DefaultSize::PLAYER_WIDTH, DefaultSize::PLAYER_HEIGHT });

//      mPlayers.push_back(player);
//      mEntities.push_back(player);
//   }
//}

//std::shared_ptr<Arena> EntityManager::GetArena() const
//{
//   return mArena;
//}

//std::vector<std::shared_ptr<Player>> EntityManager::GetPlayers() const
//{
//   return mPlayers;
//}

std::vector<std::shared_ptr<SceneObject>> EntityManager::GetEntities() const
{
   return mEntities;
}

void EntityManager::Reset()
{
   // TODO
}
