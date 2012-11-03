#include "MainMenu.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"
#include "UserInterface.hpp"
#include "MenuItem.hpp"
//#include "MenuItemSelector.hpp"

MainMenu::MainMenu(EventQueue& queue)
   : SceneObject(EntityId::Menu, ZOrder::Menu)
   , mEventQueue(queue)
{
   mEventQueue.Register(this);

   // FIXME: The mainmenu object itself is not yet constructed.
   //  This call to GetPosition() may be dangerous.
   const auto mainmenu_pos = GetPosition();

   mEventQueue.Add(std::make_shared<CreateMainMenuItemEvent>(
      MainMenuItem::ResumeGame,
      Point(mainmenu_pos.X + 224, mainmenu_pos.Y + 164 + (96 * 0)),
      Size(256, 32),
      "Resume Game",
      false,
      false)
   );

   mEventQueue.Add(std::make_shared<CreateMainMenuItemEvent>(
      MainMenuItem::NewGame,
      Point(mainmenu_pos.X + 224, mainmenu_pos.Y + 164 + (96 * 1)),
      Size(256, 32),
      "New Game",
      true,
      true)
   );

   mEventQueue.Add(std::make_shared<CreateMainMenuItemEvent>(
      MainMenuItem::Exit,
      Point(mainmenu_pos.X + 224, mainmenu_pos.Y + 164 + (96 * 2)),
      Size(256, 32),
      "Exit",
      true,
      false)
   );

   mCurrentSelection = 1; // Select "New Game" by default.
}

MainMenu::~MainMenu()
{
   mEventQueue.UnRegister(this);
}

void MainMenu::Update(const int elapsed_time)
{
   (void) elapsed_time;

   if (mInputEscape) {
      Choose(MainMenuItem::Exit);
   }
   else if (mInputEnter) {
      Choose(mItems.at(mCurrentSelection));
   }
   else if (mInputUp) {
      SelectionUp();
   }
   else if (mInputDown) {
      SelectionDown();
   }
}

void MainMenu::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::CreateMainMenuItem:
         OnCreateMainMenuItem(dynamic_cast<const CreateMainMenuItemEvent&>(event));
         break;
      case EventType::MenuInput:
         OnMenuInput(dynamic_cast<const MenuInputEvent&>(event));
         break;
      default:
         break;
   }
}

void MainMenu::OnCreateMainMenuItem(const CreateMainMenuItemEvent& event)
{
   mItems.push_back(event.GetItem());
}

void MainMenu::OnMenuInput(const MenuInputEvent& event)
{
   if (MenuType::MainMenu != event.GetMenu()) {
      return;
   }

   mInputUp = event.GetUp();
   mInputDown = event.GetDown();
   mInputLeft = event.GetLeft();
   mInputRight = event.GetRight();
   mInputEnter = event.GetEnter();
   mInputEscape = event.GetEscape();
}

MenuSound MainMenu::GetSound(const bool reset)
{
   const auto ret = mSound;

   if (reset) {
      mSound = MenuSound::None;
   }
   return ret;
}

void MainMenu::SelectionUp()
{
   mSound = MenuSound::Switch;
   const auto old_selection = mItems[mCurrentSelection];

   if (0 == mCurrentSelection) {
      mCurrentSelection = mItems.size() - 1;
   }
   else {
      mCurrentSelection--;
   }

   mEventQueue.Add(std::make_shared<MainMenuSelectionEvent>
      (old_selection, mItems[mCurrentSelection]));
}

void MainMenu::SelectionDown()
{
   mSound = MenuSound::Switch;
   const auto old_selection = mItems[mCurrentSelection];

   if ((mItems.size() - 1) == mCurrentSelection) {
      mCurrentSelection = 0;
   }
   else {
      mCurrentSelection++;
   }

   mEventQueue.Add(std::make_shared<MainMenuSelectionEvent>
      (old_selection, mItems[mCurrentSelection]));
}

void MainMenu::Choose(const MainMenuItem item)
{
   mSound = MenuSound::Choose;
   mEventQueue.Add(std::make_shared<MainMenuActionEvent>(item));
}
