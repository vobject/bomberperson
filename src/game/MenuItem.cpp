#include "MenuItem.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"

MenuItem::MenuItem(MainMenuItem type, EventQueue& queue)
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
      case EventType::MainMenuSelection:
         OnMainMenuSelection(dynamic_cast<const MainMenuSelectionEvent&>(event));
         break;
      case EventType::MainMenuEnable:
         OnMainMenuEnabled(dynamic_cast<const MainMenuEnableEvent&>(event));
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

void MenuItem::OnMainMenuSelection(const MainMenuSelectionEvent& event)
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

void MenuItem::OnMainMenuEnabled(const MainMenuEnableEvent& event)
{
   if (mType == event.GetItem())
   {
      SetEnabled(event.GetStatus());
   }
}
