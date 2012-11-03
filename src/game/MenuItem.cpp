#include "MenuItem.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"

MenuItem::MenuItem(MenuItemId type, EventQueue& queue)
   : SceneObject(EntityId::MenuItem, ZOrder::MenuItem)
   , mType(type)
   , mEventQueue(queue)
{
   mEventQueue.Register(this);
}

MenuItem::~MenuItem()
{
   mEventQueue.UnRegister(this);
}

void MenuItem::Update(const int elapsed_time)
{
   SetAnimationTime(GetAnimationTime() + elapsed_time);
}

void MenuItem::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::MenuItemSelection:
         OnMenuItemSelection(dynamic_cast<const MenuItemSelectionEvent&>(event));
         break;
      case EventType::MenuItemEnable:
         OnMenuItemEnabled(dynamic_cast<const MenuItemEnableEvent&>(event));
         break;
      default:
         break;
   }
}

std::string MenuItem::GetText() const
{
   return mText;
}

void MenuItem::SetText(const std::string& text)
{
   mText = text;
}

bool MenuItem::IsEnabled() const
{
   return mEnabled;
}

void MenuItem::SetEnabled(const bool status)
{
   mEnabled = status;
}

bool MenuItem::IsSelected() const
{
   return mSelected;
}

void MenuItem::SetSelected(bool status)
{
   mSelected = status;
}

void MenuItem::OnMenuItemSelection(const MenuItemSelectionEvent& event)
{
   if (mType == event.GetUnselected())
   {
      SetSelected(false);
   }

   if (mType == event.GetSelected())
   {
      SetSelected(true);
   }
}

void MenuItem::OnMenuItemEnabled(const MenuItemEnableEvent& event)
{
   if (mType == event.GetItem())
   {
      SetEnabled(event.GetStatus());
   }
}
