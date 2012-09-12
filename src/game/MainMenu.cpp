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
   if (0 == mSelectionMarker) {
      mSelectionMarker = mItems.size() - 1;
      return;
   }

   mSelectionMarker--;
}

void MainMenu::SelectionDown()
{
   if ((mItems.size() - 1) == mSelectionMarker) {
      mSelectionMarker = 0;
      return;
   }

   mSelectionMarker++;
}

//void MainMenu::Choose()
//{
//   mHasChosen = true;
//}

//bool MainMenu::HasChosen() const
//{
//   return mHasChosen;
//}

UserInterfaceItem MainMenu::GetSelection() const
{
   return mItems.at(mSelectionMarker);
}
