#include "MainMenu.hpp"
#include "UserInterface.hpp"

MainMenu::MainMenu()
   : SceneObject(EntityId::Menu, ZOrder::MainMenu)
{

}

MainMenu::~MainMenu()
{

}

void MainMenu::Update(const int elapsed_time)
{
   // TODO: Animate the selection marker.
}

void MainMenu::AddMenuItem(const UserInterfaceItem& item)
{
   mItems.push_back(item);
}

std::vector<UserInterfaceItem> MainMenu::GetMenuItems() const
{
   return mItems;
}

void MainMenu::SelectionUp()
{
   mSound = MenuSound::Switch;

   if (0 == mSelectionMarker) {
      mSelectionMarker = mItems.size() - 1;
      return;
   }

   mSelectionMarker--;
}

void MainMenu::SelectionDown()
{
   mSound = MenuSound::Switch;

   if ((mItems.size() - 1) == mSelectionMarker) {
      mSelectionMarker = 0;
      return;
   }

   mSelectionMarker++;
}

void MainMenu::Choose()
{
   mSound = MenuSound::Choose;
}

UserInterfaceItem MainMenu::GetSelection()
{
   return mItems.at(mSelectionMarker);
}

MenuSound MainMenu::GetSound(const bool reset)
{
   const auto ret = mSound;

   if (reset) {
      mSound = MenuSound::None;
   }
   return ret;
}
