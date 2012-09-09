#include "UserInterface.hpp"
#include "MainMenu.hpp"

#include <SDL_events.h>

UserInterface::UserInterface()
{
    mSettings = {
       {
          { PlayerId::Player_1, InputId::Keyboard_1 },
          { PlayerId::Player_2, InputId::Keyboard_2 },
          { PlayerId::Player_3, InputId::Mouse_1 }
       }
    };
}

UserInterface::~UserInterface()
{

}

void UserInterface::Input(const SDL_KeyboardEvent& key)
{
   if (SDL_KEYDOWN != key.type) {
      return;
   }

   switch (key.keysym.sym)
   {
      case SDLK_RETURN:
         mSelection = UserInterfaceItem::MainMenu_NewGame;
         mDone = true;
         break;
      case SDLK_ESCAPE:
         mSelection = UserInterfaceItem::MainMenu_Exit;
         mDone = true;
         break;
      default:
         break;
   }
}

void UserInterface::Input(const SDL_MouseMotionEvent& motion)
{

}

void UserInterface::Input(const SDL_MouseButtonEvent& button)
{

}

void UserInterface::Update(const int elapsed_time)
{
   (void) elapsed_time;
}

bool UserInterface::IsActive() const
{
   return mActive;
}

bool UserInterface::IsDone() const
{
   return mDone;
}

void UserInterface::ShowMainMenu()
{
   if (!mMainMenu)
   {
      mMainMenu = std::make_shared<MainMenu>();
   }

   mActive = true;
   mDone = false;
}

void UserInterface::HideMainMenu()
{
   mMainMenu = nullptr;
   mActive = false;
}

UserInterfaceItem UserInterface::GetSelection() const
{
  return  mSelection;
}

MatchSettings UserInterface::GetMatchSettings() const
{
   return mSettings;
}

EntitySet UserInterface::GetEntities() const
{
   return { mMainMenu };//mEntityManager.GetEntities();
}
