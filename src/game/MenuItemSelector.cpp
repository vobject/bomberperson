#include "MenuItemSelector.hpp"

MenuItemSelector::MenuItemSelector()
   : SceneObject(EntityId::MenuItemSelector, ZOrder::MenuItem)
{

}

MenuItemSelector::~MenuItemSelector()
{

}

void MenuItemSelector::Update(const int elapsed_time)
{
   SetAnimationTime(GetAnimationTime() + elapsed_time);
}
