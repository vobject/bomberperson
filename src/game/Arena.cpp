#include "Arena.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"

#include "ArenaObject.hpp"
#include "Wall.hpp"
#include "Extra.hpp"
#include "Bomb.hpp"
#include "../utils/Utils.hpp"

Arena::Arena(
   const Point pos,
   const Size size,
   const Size borders,
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
      case EventType::RemoveArena:
         OnRemoveArena(dynamic_cast<const RemoveArenaEvent&>(event));
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

      case EventType::BombCellChanged:
         OnBombCellChanged(dynamic_cast<const BombCellChangedEvent&>(event));
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

bool Arena::HasWall(const Cell cell) const
{
   return mCells.at(cell.X).at(cell.Y).second.wall;
}

WallType Arena::GetWallType(const Cell cell) const
{
   return mCells.at(cell.X).at(cell.Y).second.wall_type;
}

bool Arena::HasExtra(const Cell cell) const
{
   return mCells.at(cell.X).at(cell.Y).second.extra;
}

ExtraType Arena::GetExtraType(const Cell cell) const
{
   return mCells.at(cell.X).at(cell.Y).second.extra_type;
}

bool Arena::HasBomb(const Cell cell) const
{
   return mCells.at(cell.X).at(cell.Y).second.bomb;
}

BombType Arena::GetBombType(const Cell cell) const
{
   return mCells.at(cell.X).at(cell.Y).second.bomb_type;
}

bool Arena::HasExplosion(const Cell cell) const
{
   return mCells.at(cell.X).at(cell.Y).second.explosion;
}

PlayerType Arena::GetExplosionOwner(const Cell cell) const
{
   return mCells.at(cell.X).at(cell.Y).second.explosion_owner;
}

void Arena::OnRemoveArena(const RemoveArenaEvent& event)
{
   (void) event;

   Invalidate();
}

void Arena::OnCreateWall(const CreateWallEvent& event)
{
   if (HasWall(event.GetCell())) {
      throw "Trying to create a wall on a cell that already has one.";
   }
   CreateWall(event.GetCell(), event.GetWall());
}

void Arena::OnCreateExtra(const CreateExtraEvent& event)
{
   if (HasExtra(event.GetCell())) {
      throw "Trying to create an extra on a cell that already has one.";
   }
   CreateExtra(event.GetCell(), event.GetExtra());
}

void Arena::OnCreateBomb(const CreateBombEvent& event)
{
   if (HasBomb(event.GetCell())) {
      throw "Trying to create a bomb on a cell that already has one.";
   }
   CreateBomb(event.GetCell(), event.GetBomb());
}

void Arena::OnCreateExplosion(const CreateExplosionEvent& event)
{
   // Explosions may overlap.
   // This behaviour favours the latest explosion. This might not be fair
   //  (no first-come-first-served) but is easier to implement. The latest
   //  explosion on a cell is guarantied to burn the longest and therefore
   //  kill players as long as there is an explosion on the cell.
   CreateExplosion(event.GetCell(), event.GetOwner());
}

void Arena::OnRemoveWall(const RemoveWallEvent& event)
{
   RemoveWall(event.GetCell());
}

void Arena::OnRemoveExtra(const RemoveExtraEvent& event)
{
   RemoveExtra(event.GetCell());
}

void Arena::OnRemoveBomb(const RemoveBombEvent& event)
{
   RemoveBomb(event.GetCell());
}

void Arena::OnRemoveExplosion(const RemoveExplosionEvent& event)
{
   RemoveExplosion(event.GetCell());
}

void Arena::OnBombCellChanged(const BombCellChangedEvent& event)
{
   const auto old_cell = event.GetOldCell();
   const auto new_cell = event.GetNewCell();

   if (!HasBomb(old_cell)) {
      throw "Cannot change the parent cell for a non-existend bomb.";
   }

   if (HasBomb(new_cell)) {
      throw "The new parent cell already contains a bomb.";
   }

   const auto type = GetBombType(old_cell);
   RemoveBomb(old_cell);
   CreateBomb(new_cell, type);
}

void Arena::CreateWall(const Cell cell, const WallType type)
{
   mCells.at(cell.X).at(cell.Y).second.wall = true;
   mCells.at(cell.X).at(cell.Y).second.wall_type = type;
}

void Arena::CreateExtra(const Cell cell, const ExtraType type)
{
   mCells.at(cell.X).at(cell.Y).second.extra = true;
   mCells.at(cell.X).at(cell.Y).second.extra_type = type;
}

void Arena::CreateBomb(const Cell cell, const BombType type)
{
   mCells.at(cell.X).at(cell.Y).second.bomb = true;
   mCells.at(cell.X).at(cell.Y).second.bomb_type = type;
}

void Arena::CreateExplosion(const Cell cell, const PlayerType owner)
{
   mCells.at(cell.X).at(cell.Y).second.explosion = true;
   mCells.at(cell.X).at(cell.Y).second.explosion_owner = owner;
}

void Arena::RemoveWall(const Cell cell)
{
   mCells.at(cell.X).at(cell.Y).second.wall = false;
}

void Arena::RemoveExtra(const Cell cell)
{
   mCells.at(cell.X).at(cell.Y).second.extra = false;
}

void Arena::RemoveBomb(const Cell cell)
{
   mCells.at(cell.X).at(cell.Y).second.bomb = false;
}

void Arena::RemoveExplosion(const Cell cell)
{
   mCells.at(cell.X).at(cell.Y).second.explosion = false;
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
