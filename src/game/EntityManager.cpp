#include "EntityManager.hpp"
#include "EntityId.hpp"
#include "Arena.hpp"
#include "Bomb.hpp"
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

std::shared_ptr<Bomb> EntityManager::CreateBomb(const std::shared_ptr<Cell>& cell)
{
   auto bomb = std::make_shared<Bomb>(*this, cell);
   bomb->SetSize(cell->GetSize());
   bomb->SetPosition(cell->GetPosition());

   mBombs.push_back(bomb);
   mEntities.push_back(bomb);
   return bomb;
}

std::shared_ptr<Explosion> EntityManager::CreateExplosion(const std::shared_ptr<Cell>& cell)
{
   auto explosion = std::make_shared<Explosion>();
   explosion->SetSize(cell->GetSize());
   explosion->SetPosition(cell->GetPosition());

   mExplosions.push_back(explosion);
   mEntities.push_back(explosion);
   return explosion;
}

void EntityManager::CreatePlayers(
   const int count,
   const std::shared_ptr<Arena>& arena
)
{
   for (int i = 0; i < count; i++)
   {
      std::shared_ptr<Player> player = nullptr;
      std::shared_ptr<Cell> parent_cell = nullptr;

      switch (i)
      {
         case 0:
            player = std::make_shared<Player>(EntityId::Player_1, *this);
            parent_cell = arena->GetCellFromCoordinates(DefaultSize::PLAYER_1_CELL_X, DefaultSize::PLAYER_1_CELL_Y);
            break;
         case 1:
            player = std::make_shared<Player>(EntityId::Player_2, *this);
            parent_cell = arena->GetCellFromCoordinates(DefaultSize::PLAYER_2_CELL_X, DefaultSize::PLAYER_2_CELL_Y);
            break;
         case 2:
            player = std::make_shared<Player>(EntityId::Player_3, *this);
            parent_cell = arena->GetCellFromCoordinates(DefaultSize::PLAYER_3_CELL_X, DefaultSize::PLAYER_3_CELL_Y);
            break;
         case 3:
            player = std::make_shared<Player>(EntityId::Player_4, *this);
            parent_cell = arena->GetCellFromCoordinates(DefaultSize::PLAYER_4_CELL_X, DefaultSize::PLAYER_4_CELL_Y);
            break;
         default:
            LOG(logERROR) << "Trying to create more players than supported.";
            return;
      }

      player->SetParentCell(parent_cell);
      player->SetPosition(parent_cell->GetPosition());
      player->SetSize({ DefaultSize::PLAYER_WIDTH, DefaultSize::PLAYER_HEIGHT });

      mPlayers.push_back(player);
      mEntities.push_back(player);
   }
}

std::vector<std::shared_ptr<Player>> EntityManager::GetPlayers() const
{
   return mPlayers;
}

std::vector<std::shared_ptr<SceneObject>> EntityManager::GetEntities() const
{
   return mEntities;
}

void EntityManager::Reset()
{
   // TODO
}
