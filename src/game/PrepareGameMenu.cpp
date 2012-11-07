#include "PrepareGameMenu.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"
#include "UserInterface.hpp"
#include "MenuItem.hpp"

PrepareGameMenu::PrepareGameMenu(EventQueue& queue)
   : SceneObject(EntityId::Menu, ZOrder::Menu)
   , mEventQueue(queue)
{
   mEventQueue.Register(this);

   // FIXME: The mainmenu object itself is not yet constructed.
   //  This call to GetPosition() may be dangerous.
   const auto pos = GetPosition();

   mEventQueue.Add(std::make_shared<CreateMenuItemEvent>(
      MenuType::PrepareGame,
      MenuItemId::PrepareGame_Arena,
      Point(pos.X + 224, pos.Y + 164 + (96 * 0)),
      Size(256, 32),
      "Test 1",
      false)
   );

   mEventQueue.Add(std::make_shared<CreateMenuItemEvent>(
      MenuType::PrepareGame,
      MenuItemId::PrepareGame_ArenaSize,
      Point(pos.X + 224, pos.Y + 164 + (96 * 1)),
      Size(256, 32),
      "Test 2",
      true)
   );

   mEventQueue.Add(std::make_shared<CreateMenuItemEvent>(
      MenuType::PrepareGame,
      MenuItemId::PrepareGame_PlayerCount,
      Point(pos.X + 224, pos.Y + 164 + (96 * 2)),
      Size(256, 32),
      "Test 3",
      true)
   );

   mCurrentSelection = 0;
   mEventQueue.Add(std::make_shared<MenuItemSelectionEvent>
      (MenuItemId::PrepareGame_Arena, MenuItemId::PrepareGame_Arena));
}

PrepareGameMenu::~PrepareGameMenu()
{
   mEventQueue.UnRegister(this);
}

void PrepareGameMenu::Update(const int elapsed_time)
{
   (void) elapsed_time;

//   if (mInputEscape) {
//      Choose(MenuItemId::MainMenu_Exit);
//   }
//   else if (mInputEnter) {
//      Choose(mItems.at(mCurrentSelection));
//   }
//   else if (mInputUp) {
//      SelectionUp();
//   }
//   else if (mInputDown) {
//      SelectionDown();
//   }
}

void PrepareGameMenu::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::CreateMenuItem:
         OnCreateMenuItem(dynamic_cast<const CreateMenuItemEvent&>(event));
         break;
      case EventType::MenuInput:
         OnMenuInput(dynamic_cast<const MenuInputEvent&>(event));
         break;
      case EventType::MenuItemAction:
         OnMenuItemAction(dynamic_cast<const MenuItemActionEvent&>(event));
         break;
      default:
         break;
   }
}

void PrepareGameMenu::OnCreateMenuItem(const CreateMenuItemEvent& event)
{
   if (MenuType::PrepareGame != event.GetOwner()) {
      return;
   }

   mItems.push_back(event.GetItem());
}

void PrepareGameMenu::OnMenuInput(const MenuInputEvent& event)
{
   if (MenuType::PrepareGame != event.GetTarget()) {
      return;
   }

   mInputUp = event.GetUp();
   mInputDown = event.GetDown();
   mInputLeft = event.GetLeft();
   mInputRight = event.GetRight();
   mInputEnter = event.GetEnter();
   mInputEscape = event.GetEscape();
}

void PrepareGameMenu::OnMenuItemAction(const MenuItemActionEvent& event)
{

}

void PrepareGameMenu::SelectionUp()
{
   const auto old_selection = mItems[mCurrentSelection];

   if (0 == mCurrentSelection) {
      mCurrentSelection = mItems.size() - 1;
   }
   else {
      mCurrentSelection--;
   }

   mEventQueue.Add(std::make_shared<MenuItemSelectionEvent>
      (old_selection, mItems[mCurrentSelection]));
}

void PrepareGameMenu::SelectionDown()
{
   const auto old_selection = mItems[mCurrentSelection];

   if ((mItems.size() - 1) == mCurrentSelection) {
      mCurrentSelection = 0;
   }
   else {
      mCurrentSelection++;
   }

   mEventQueue.Add(std::make_shared<MenuItemSelectionEvent>
      (old_selection, mItems[mCurrentSelection]));
}

void PrepareGameMenu::Choose(const MenuItemId item)
{
   mEventQueue.Add(std::make_shared<MenuItemActionEvent>(item));
}
