#include "Cell.hpp"
#include "Arena.hpp"
#include "Wall.hpp"
#include "Extra.hpp"
#include "Bomb.hpp"
#include "Explosion.hpp"
#include "../utils/Utils.hpp"

Cell::Cell(
   const int field_pos_x,
   const int field_pos_y,
   const std::shared_ptr<Arena>& field
)
   : SceneObject(EntityId::Cell)
   , mFieldPosX(field_pos_x)
   , mFieldPosY(field_pos_y)
   , mField(field) // Do not use inside the ctor - object is not ready yet.
{

}

Cell::~Cell()
{

}

void Cell::Update(const int elapsed_time)
{
   if (mWall)
   {
      mWall->Update(elapsed_time);

      if (!mWall->IsAlive()) {
         mWall = nullptr;
      }
   }

   if (mExtra)
   {
      mExtra->Update(elapsed_time);

      if (!mExtra->IsAlive()) {
         mExtra = nullptr;
      }
   }

   if (mBomb)
   {
      mBomb->Update(elapsed_time);

      if (!mBomb->IsAlive()) {
         mBomb = nullptr;
      }
   }

   if (mExplosion)
   {
      mExplosion->Update(elapsed_time);

      if (!mExplosion->IsAlive()) {
         mExplosion = nullptr;
      }
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

std::shared_ptr<Extra> Cell::CollectExtra()
{
   // Return the current extra item and remove it from the cell.
   // Do not use DestroyExtra() as the item is not destroyed, it will
   //  just be transfered from the cell to the caller (probably the player).
   const auto extra = mExtra;
   mExtra = nullptr;
   return extra;
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
         return mField->GetCellAboveOf(mFieldPosX, mFieldPosY);
      case Direction::Down:
         return mField->GetCellBelowOf(mFieldPosX, mFieldPosY);
      case Direction::Left:
         return mField->GetCellLeftOf(mFieldPosX, mFieldPosY);
      case Direction::Right:
         return mField->GetCellRightOf(mFieldPosX, mFieldPosY);
   }
   return nullptr;
}
