#include "Extra.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"

Extra::Extra(
   const std::shared_ptr<Arena>& arena,
   const ExtraType type,
   EventQueue& queue
)
   : ArenaObject(EntityId::Extra, ZOrder::Layer_2, arena)
   , mType(type)
   , mEventQueue(queue)
{
   mEventQueue.Register(this);
}

Extra::~Extra()
{
   mEventQueue.UnRegister(this);
}

void Extra::Update(const int elapsed_time)
{
   (void) elapsed_time;
}

void Extra::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::RemoveExtra:
         OnRemoveExtra(dynamic_cast<const RemoveExtraEvent&>(event));
         break;
      default:
         break;
   }
}

ExtraType Extra::GetType() const
{
   return mType;
}

void Extra::OnRemoveExtra(const RemoveExtraEvent& event)
{
   const auto parent_cell = GetArena()->GetCellFromObject(*this);

   if (event.GetCell() != parent_cell) {
      return;
   }

   Invalidate();
}
