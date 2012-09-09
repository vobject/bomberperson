#include "EntityManager.hpp"
#include "EntityId.hpp"
#include "Arena.hpp"
#include "ArenaGenerator.hpp"
#include "Cell.hpp"
#include "Wall.hpp"
#include "Extra.hpp"
#include "Bomb.hpp"
#include "Explosion.hpp"
#include "Player.hpp"
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

   mEntities.insert(arena);
   return arena;
}

std::shared_ptr<Cell> EntityManager::CreateCell(
   const std::shared_ptr<Arena>& arena,
   const int x,
   const int y
)
{
   auto cell = std::make_shared<Cell>(arena, x, y);

   mEntities.insert(cell);
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

   mEntities.insert(wall);
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

   mEntities.insert(extra);
   return extra;
}

std::shared_ptr<Bomb> EntityManager::CreateBomb(const std::shared_ptr<Cell>& cell)
{
   auto bomb = std::make_shared<Bomb>(*this, cell);
   bomb->SetPosition(cell->GetPosition());
   bomb->SetSize(cell->GetSize());

   mEntities.insert(bomb);
   return bomb;
}

std::shared_ptr<Explosion> EntityManager::CreateExplosion(const std::shared_ptr<Cell>& cell)
{
   auto explosion = std::make_shared<Explosion>();
   explosion->SetPosition(cell->GetPosition());
   explosion->SetSize(cell->GetSize());

   mEntities.insert(explosion);
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

   mEntities.insert(player);
   return player;
}

EntitySet EntityManager::GetEntities() const
{
   return mEntities;
}

void EntityManager::Reset()
{
   mEntities.clear();
}
