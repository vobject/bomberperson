#include "Wall.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"

Wall::Wall(
   const std::shared_ptr<Arena>& arena,
   const WallType type,
   EventQueue& queue
)
   : ArenaObject(EntityId::Wall, ZOrder::Layer_3, arena)
   , mType(type)
   , mEventQueue(queue)
{
   mEventQueue.Register(this);
}

Wall::~Wall()
{
   mEventQueue.UnRegister(this);
}

void Wall::Update(const int elapsed_time)
{
   (void) elapsed_time;
}

void Wall::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::RemoveWall:
         OnRemoveWall(dynamic_cast<const RemoveWallEvent&>(event));
         break;
      default:
         break;
   }
}

WallType Wall::GetType() const
{
   return mType;
}

void Wall::OnRemoveWall(const RemoveWallEvent& event)
{
   const auto parent_cell = GetArena()->GetCellFromObject(*this);

   if (event.GetCell() != parent_cell) {
      return;
   }

   Invalidate();
}
