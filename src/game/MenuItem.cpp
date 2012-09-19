#include "MenuItem.hpp"

MenuItem::MenuItem(const UiItemId id)
   : SceneObject(EntityId::Menu, ZOrder::MenuItem)
   , mId(id)
{

}

MenuItem::~MenuItem()
{

}

void MenuItem::Update(const int elapsed_time)
{
   (void) elapsed_time;
}

UiItemId MenuItem::GetItemId() const
{
   return mId;
}

bool MenuItem::IsEnabled() const
{
   return mEnabled;
}

void MenuItem::SetEnabled(const bool status)
{
   mEnabled = status;
}

std::string MenuItem::GetText() const
{
   return mText;
}

void MenuItem::SetText(const std::string& text)
{
   mText = text;
}
