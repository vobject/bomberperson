#include "Cell.hpp"
//#include "Arena.hpp"
//#include "Wall.hpp"
//#include "Extra.hpp"
//#include "Bomb.hpp"
//#include "Explosion.hpp"
//#include "../utils/Utils.hpp"

//Cell::Cell(
//   const std::shared_ptr<Arena>& arena,
//   const int arena_pos_x,
//   const int arena_pos_y
//)
//   : ArenaObject(EntityId::Cell, ZOrder::Layer_3, arena, nullptr)
//   , mXCell(arena_pos_x)
//   , mYCell(arena_pos_y)
//{

//}

//Cell::~Cell()
//{
//   // When a cell object is destroyed make sure everything
//   //  that is present on the cell is no longer valid.

//   if (mWall) {
//      mWall->Invalidate();
//   }

//   if (mExtra) {
//      mExtra->Invalidate();
//   }

//   if (mBomb) {
//      mBomb->Invalidate();
//   }

//   if (mExplosion) {
//      mExplosion->Invalidate();
//   }
//}

//void Cell::Update(const int elapsed_time)
//{
//   (void) elapsed_time;

//   // Cleanup the objects on this cell where necessary.

//   if (mWall && !mWall->IsValid()) {
//         mWall = nullptr;
//   }

//   if (mExtra && !mExtra->IsValid()) {
//      mExtra = nullptr;
//   }

//   if (mBomb && !mBomb->IsValid()) {
//      mBomb = nullptr;
//   }

//   if (mExplosion && !mExplosion->IsValid()) {
//      mExplosion = nullptr;
//   }
//}

//bool Cell::HasWall() const
//{
//   return (mWall != nullptr);
//}

//std::shared_ptr<Wall> Cell::GetWall() const
//{
//   return mWall;
//}

//void Cell::SetWall(const std::shared_ptr<Wall>& wall)
//{
//   mWall = wall;
//}

//void Cell::DestroyWall()
//{
//   mWall->Invalidate();
//}

//bool Cell::HasExtra() const
//{
//   return (mExtra != nullptr);
//}

//std::shared_ptr<Extra> Cell::GetExtra() const
//{
//   return mExtra;
//}

//void Cell::SetExtra(const std::shared_ptr<Extra>& extra)
//{
//   mExtra = extra;
//}

//void Cell::DestroyExtra()
//{
//   mExtra->Invalidate();
//}

//bool Cell::HasBomb() const
//{
//   return (mBomb != nullptr);
//}

//std::shared_ptr<Bomb> Cell::GetBomb() const
//{
//   return mBomb;
//}

//void Cell::SetBomb(const std::shared_ptr<Bomb>& bomb)
//{
//   mBomb = bomb;
//}

//void Cell::DetonateBomb()
//{
//   mBomb->Detonate();
//}

//bool Cell::HasExplosion() const
//{
//   return (mExplosion != nullptr);
//}

//std::shared_ptr<Explosion> Cell::GetExplosion() const
//{
//   return mExplosion;
//}

//void Cell::SetExplosion(const std::shared_ptr<Explosion>& explosion)
//{
//   mExplosion = explosion;
//}

//std::shared_ptr<Cell> Cell::GetNeighborCell(const Direction dir) const
//{
//   switch (dir)
//   {
//      case Direction::Up:
//         return mArena->GetCellAboveOf(mXCell, mYCell);
//      case Direction::Down:
//         return mArena->GetCellBelowOf(mXCell, mYCell);
//      case Direction::Left:
//         return mArena->GetCellLeftOf(mXCell, mYCell);
//      case Direction::Right:
//         return mArena->GetCellRightOf(mXCell, mYCell);
//   }
//   return nullptr;
//}
