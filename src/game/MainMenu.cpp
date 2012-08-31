#include "MainMenu.hpp"

MainMenu::MainMenu(const std::string &name)
{
   SetResourceId(name);
}

MainMenu::~MainMenu()
{

}

void MainMenu::Update(const int elapsed_time)
{

}

bool MainMenu::HasChosen() const
{
   return mHasChosen;
}

MainMenuItem MainMenu::GetSelection() const
{
   return mSelection;
}

void MainMenu::SelectionUp()
{
   if (FIRST_ITEM == mSelection) {
      mSelection = LAST_ITEM;
      return;
   }

   mSelection = static_cast<MainMenuItem>(static_cast<int>(mSelection) - 1);
}

void MainMenu::SelectionDown()
{
   if (LAST_ITEM == mSelection) {
      mSelection = FIRST_ITEM;
      return;
   }

   mSelection = static_cast<MainMenuItem>(static_cast<int>(mSelection) + 1);
}

void MainMenu::Choose()
{
   mHasChosen = true;
}
