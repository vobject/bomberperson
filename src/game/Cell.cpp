#include "Cell.hpp"
#include "Arena.hpp"
#include "Wall.hpp"
#include "Extra.hpp"
#include "Bomb.hpp"
#include "Explosion.hpp"
#include "../utils/Utils.hpp"

Cell::Cell(
   const std::shared_ptr<Arena>& arena,
   const int arena_pos_x,
   const int arena_pos_y
)
   : SceneObject(EntityId::Cell)
   , mArenaPosX(arena_pos_x)
   , mArenaPosY(arena_pos_y)
   , mArena(arena) // Do not use inside the ctor because
                   //  object itself is not fully constructed yet.
{
   SetZOrder(ZOrder::Layer_3);
}

Cell::~Cell()
{
   // When a cell object is destroyed make sure everything
   //  that is present on the cell also dies.

   if (mWall) {
      mWall->SetAlive(false);
   }

   if (mExtra) {
      mExtra->SetAlive(false);
   }

   if (mBomb) {
      mBomb->SetAlive(false);
   }

   if (mExplosion) {
      mExplosion->SetAlive(false);
   }
}

void Cell::Update(const int elapsed_time)
{
   if (mWall && !mWall->IsAlive()) {
         mWall = nullptr;
   }

   if (mExtra && !mExtra->IsAlive()) {
      mExtra = nullptr;
   }

   if (mBomb && !mBomb->IsAlive()) {
      mBomb = nullptr;
   }

   if (mExplosion && !mExplosion->IsAlive()) {
      mExplosion = nullptr;
   }
}

bool Cell::HasWall() const
{
   return (mWall != nullptr);
}

std::shared_ptr<Wall> Cell::GetWall() const
{
   return mWall;
}

void Cell::SetWall(const std::shared_ptr<Wall>& wall)
{
   mWall = wall;
}

void Cell::DestroyWall()
{
   mWall->SetAlive(false);
}

bool Cell::HasExtra() const
{
   return (mExtra != nullptr);
}

std::shared_ptr<Extra> Cell::GetExtra() const
{
   return mExtra;
}

void Cell::SetExtra(const std::shared_ptr<Extra>& extra)
{
   mExtra = extra;
}

void Cell::DestroyExtra()
{
   mExtra->SetAlive(false);
}

bool Cell::HasBomb() const
{
   return (mBomb != nullptr);
}

std::shared_ptr<Bomb> Cell::GetBomb() const
{
   return mBomb;
}

void Cell::SetBomb(const std::shared_ptr<Bomb>& bomb)
{
   mBomb = bomb;
}

void Cell::DetonateBomb()
{
   mBomb->Detonate();
}

bool Cell::HasExplosion() const
{
   return (mExplosion != nullptr);
}

std::shared_ptr<Explosion> Cell::GetExplosion() const
{
   return mExplosion;
}

void Cell::SetExplosion(const std::shared_ptr<Explosion>& explosion)
{
   mExplosion = explosion;
}

std::shared_ptr<Cell> Cell::GetNeighborCell(const Direction dir) const
{
   switch (dir)
   {
      case Direction::Up:
         return mArena->GetCellAboveOf(mArenaPosX, mArenaPosY);
      case Direction::Down:
         return mArena->GetCellBelowOf(mArenaPosX, mArenaPosY);
      case Direction::Left:
         return mArena->GetCellLeftOf(mArenaPosX, mArenaPosY);
      case Direction::Right:
         return mArena->GetCellRightOf(mArenaPosX, mArenaPosY);
   }
   return nullptr;
}
