#include "MainMenu.hpp"
#include "UserInterface.hpp"
#include "MenuItem.hpp"
#include "MenuItemSelector.hpp"

MainMenu::MainMenu(EntityManager& entity_factory)
   : SceneObject(EntityId::Menu, ZOrder::Menu)
   , mEntityFactory(entity_factory)
{
   // TODO: The mainmenu object itself is not yet constructed.
   //  This call to GetPosition() may be dangerous.
   const auto mainmenu_pos = GetPosition();

   mSelector = mEntityFactory.CreateMenuItemSelector();
   mSelector->SetPosition({ mainmenu_pos.X + 144, mainmenu_pos.Y + 158 + (96 * 1) });

   auto resume_game = mEntityFactory.CreateMenuItem(UiItemId::MainMenu_ResumeGame);
   resume_game->SetPosition({ mainmenu_pos.X + 224, mainmenu_pos.Y + 164 + (96 * 0) });
   resume_game->SetEnabled(false);
   resume_game->SetText("Resume Game");
   mItems.push_back(resume_game);

   auto new_game = mEntityFactory.CreateMenuItem(UiItemId::MainMenu_NewGame);
   new_game->SetPosition({ mainmenu_pos.X + 224, mainmenu_pos.Y + 164 + (96 * 1) });
   new_game->SetEnabled(true);
   new_game->SetText("New Game");
   mItems.push_back(new_game);

   auto exit_app = mEntityFactory.CreateMenuItem(UiItemId::MainMenu_Exit);
   exit_app->SetPosition({ mainmenu_pos.X + 224, mainmenu_pos.Y + 164 + (96 * 2) });
   exit_app->SetEnabled(true);
   exit_app->SetText("Exit");
   mItems.push_back(exit_app);

   mSelectionMarker = 1; // Select "New Game" by default.
}

MainMenu::~MainMenu()
{

}

void MainMenu::Update(const int elapsed_time)
{
   // TODO: Animate the menu background.
}

void MainMenu::SelectionUp()
{
   mSound = MenuSound::Switch;

   if (0 == mSelectionMarker) {
      mSelectionMarker = mItems.size() - 1;
   }
   else {
      mSelectionMarker--;
   }

   if (mItems[mSelectionMarker]->IsEnabled())
   {
      mSelector->SetPosition({ mSelector->GetPosition().X,
                               mItems[mSelectionMarker]->GetPosition().Y });
   }
   else
   {
      SelectionUp();
   }
}

void MainMenu::SelectionDown()
{
   mSound = MenuSound::Switch;

   if ((mItems.size() - 1) == mSelectionMarker) {
      mSelectionMarker = 0;
   }
   else {
      mSelectionMarker++;
   }

   if (mItems[mSelectionMarker]->IsEnabled())
   {
      mSelector->SetPosition({ mSelector->GetPosition().X,
                               mItems[mSelectionMarker]->GetPosition().Y - 8 });
   }
   else
   {
      SelectionDown();
   }
}

void MainMenu::Choose()
{
   mSound = MenuSound::Choose;
}

std::shared_ptr<MenuItem> MainMenu::GetSelection()
{
   return mItems[mSelectionMarker];
}

std::vector<std::shared_ptr<MenuItem>> MainMenu::GetMenuItems() const
{
   return mItems;
}

MenuSound MainMenu::GetSound(const bool reset)
{
   const auto ret = mSound;

   if (reset) {
      mSound = MenuSound::None;
   }
   return ret;
}

void MainMenu::SetResumeStatus(const bool enabled)
{
   mItems[0]->SetEnabled(enabled);
}
