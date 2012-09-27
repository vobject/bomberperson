#include "Arena.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"

#include "ArenaObject.hpp"
#include "Wall.hpp"
#include "Extra.hpp"
#include "Bomb.hpp"
#include "Explosion.hpp"
#include "../utils/Utils.hpp"

Arena::Arena(
   const Point& pos,
   const Size& size,
   const Size& borders,
   const int cells_x,
   const int cells_y,
   EventQueue& queue
)
   : SceneObject(EntityId::Arena, ZOrder::Layer_1)
   , mXCells(cells_x)
   , mYCells(cells_y)
   , mBorders(borders)
   , mCellSize((size.Width - (mBorders.Width * 2)) / mXCells,
               (size.Height - (mBorders.Height * 2)) / mYCells)
   , mCells(mXCells)
   , mEventQueue(queue)
{
   SetPosition(pos);
   SetSize(size);

   mEventQueue.Register(this);

   // We now have all the information needed to initialize the cells.

   for (int x = 0; x < mXCells; x++)
   {
      for (int y = 0; y < mYCells; y++)
      {
         mCells[x].push_back({{ x, y }, CellContent()});

         // The initial arena does not contain any objects like walls,
         //  extras or players. Those will be set from the outside.
      }
   }
}

Arena::~Arena()
{
   mEventQueue.UnRegister(this);
}

void Arena::Update(const int elapsed_time)
{
   // TODO: May be used for an animated ground (e.g. grass or craters).
}

void Arena::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::ParentCellChanged:
         OnParentCellChanged(dynamic_cast<const ParentCellChangedEvent&>(event));
         break;
      default:
         break;
   }
}

void Arena::SetObjectPosition(ArenaObject& obj, const Cell& cell) const
{
   obj.SetPosition(GetCellPosition(cell));
}

void Arena::SetObjectSize(ArenaObject& obj) const
{
   obj.SetSize({ mCellSize.Width, mCellSize.Height });
}

Size Arena::GetCellSize() const
{
   return mCellSize;
}

Point Arena::GetCellPosition(const Cell& cell) const
{
   return { GetPosition().X + mBorders.Width + (mCellSize.Width * cell.X),
            GetPosition().Y + mBorders.Height + (mCellSize.Height * cell.Y) };
}

Cell Arena::GetCellFromObject(const ArenaObject& obj) const
{
   // An object's position is its center.
   const Point pos = { obj.GetPosition().X + (obj.GetSize().Width / 2),
                       obj.GetPosition().Y + (obj.GetSize().Height / 2) };
   return GetCellFromPosition(pos);
}

Cell Arena::GetCellFromPosition(const Point& pos) const
{
   const auto cell_x = (pos.X - GetPosition().X - mBorders.Width) / mCellSize.Width;
   const auto cell_y = (pos.Y - GetPosition().Y - mBorders.Height) / mCellSize.Height;
   return GetCellFromCoordinates(cell_x, cell_y);
}

Cell Arena::GetCellFromCoordinates(const int cell_x, const int cell_y) const
{
   return mCells.at(cell_x).at(cell_y).first;
}

Cell Arena::GetNeighborCell(const Cell& cell, const Direction dir) const
{
   switch (dir)
   {
      case Direction::Up:
         return GetCellAboveOf(cell.X, cell.Y);
      case Direction::Down:
         return GetCellBelowOf(cell.X, cell.Y);
      case Direction::Left:
         return GetCellLeftOf(cell.X, cell.Y);
      case Direction::Right:
         return GetCellRightOf(cell.X, cell.Y);
   }
   return { -1, -1 };
}

bool Arena::HasWall(const Cell& cell) const
{
   const auto obj = mCells.at(cell.X).at(cell.Y).second.wall;
   return (obj != nullptr) && obj->IsValid();
}

std::shared_ptr<Wall> Arena::GetWall(const Cell& cell) const
{
   return mCells.at(cell.X).at(cell.Y).second.wall;
}

void Arena::SetWall(const Cell& cell, const std::shared_ptr<Wall>& wall)
{
   mCells.at(cell.X).at(cell.Y).second.wall = wall;
}

void Arena::DestroyWall(const Cell& cell)
{
   mCells.at(cell.X).at(cell.Y).second.wall->Invalidate();
}

bool Arena::HasExtra(const Cell& cell) const
{
   const auto obj = mCells.at(cell.X).at(cell.Y).second.extra;
   return (obj != nullptr) && obj->IsValid();
}

std::shared_ptr<Extra> Arena::GetExtra(const Cell& cell) const
{
   return mCells.at(cell.X).at(cell.Y).second.extra;
}

void Arena::SetExtra(const Cell& cell, const std::shared_ptr<Extra>& extra)
{
   mCells.at(cell.X).at(cell.Y).second.extra = extra;
}

bool Arena::HasBomb(const Cell& cell) const
{
   const auto obj = mCells.at(cell.X).at(cell.Y).second.bomb;
   return (obj != nullptr) && obj->IsValid();
}

std::shared_ptr<Bomb> Arena::GetBomb(const Cell& cell) const
{
   return mCells.at(cell.X).at(cell.Y).second.bomb;
}

void Arena::SetBomb(const Cell& cell, const std::shared_ptr<Bomb>& bomb)
{
   mCells.at(cell.X).at(cell.Y).second.bomb = bomb;
}

bool Arena::HasExplosion(const Cell& cell) const
{
   const auto obj = mCells.at(cell.X).at(cell.Y).second.explosion;
   return (obj != nullptr) && obj->IsValid();
}

std::shared_ptr<Explosion> Arena::GetExplosion(const Cell& cell) const
{
   return mCells.at(cell.X).at(cell.Y).second.explosion;
}

void Arena::SetExplosion(const Cell& cell, const std::shared_ptr<Explosion>& explosion)
{
   mCells.at(cell.X).at(cell.Y).second.explosion = explosion;
}

void Arena::OnParentCellChanged(const ParentCellChangedEvent& event)
{
   // Currently only bombs can change their parent cell.

   if (!HasBomb(event.GetOldCell())) {
      throw "Cannot change the parent cell for a non-existend bomb.";
   }

   if (HasBomb(event.GetNewCell())) {
      throw "The new parent cell already contains a bomb.";
   }

   const auto bomb = GetBomb(event.GetOldCell());
   SetBomb(event.GetOldCell(), nullptr);
   SetBomb(event.GetNewCell(), bomb);
}

Cell Arena::GetCellAboveOf(const int cell_x, const int cell_y) const
{
   if (cell_y <= 0) {
      return { -1, -1 };
   }
   return GetCellFromCoordinates(cell_x, cell_y - 1);
}

Cell Arena::GetCellBelowOf(const int cell_x, const int cell_y) const
{
   if (cell_y == (mYCells - 1)) {
      return { -1, -1 };
   }
   return GetCellFromCoordinates(cell_x, cell_y + 1);
}

Cell Arena::GetCellLeftOf(const int cell_x, const int cell_y) const
{
   if (cell_x <= 0) {
      return { -1, -1 };
   }
   return GetCellFromCoordinates(cell_x - 1, cell_y);
}

Cell Arena::GetCellRightOf(const int cell_x, const int cell_y) const
{
   if (cell_x == (mXCells - 1)) {
      return { -1, -1 };
   }
   return GetCellFromCoordinates(cell_x + 1, cell_y);
}
