#include "UserInterface.hpp"
#include "MainMenu.hpp"
#include "MenuItem.hpp"

#include <SDL_events.h>

UserInterface::UserInterface()
   : mEntityManager(mEventQueue)
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
      case SDLK_UP:
         mMainMenu->SelectionUp();
         break;
      case SDLK_DOWN:
         mMainMenu->SelectionDown();
         break;
      case SDLK_RETURN:
         mMainMenu->Choose();
         mSelection = mMainMenu->GetSelection()->GetItemId();
         mDone = true;
         break;
      case SDLK_BACKSPACE:
         mSelection = UiItemId::MainMenu_ResumeGame;
         mDone = true;
         break;
      case SDLK_ESCAPE:
         mSelection = UiItemId::MainMenu_Exit;
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
   for (auto& ent : mEntityManager.GetEntities())
   {
      if (!ent->IsValid()) {
         continue;
      }
      ent->Update(elapsed_time);
   }
}

bool UserInterface::IsActive() const
{
   return mActive;
}

bool UserInterface::IsDone() const
{
   return mDone;
}

void UserInterface::ShowMainMenu(const bool game_paused)
{
   if (!mMainMenu)
   {
      mMainMenu = mEntityManager.CreateMainmenu();
      mSelection = mMainMenu->GetSelection()->GetItemId();
   }

   if (game_paused)
   {
      mMainMenu->SetResumeStatus(true);

      // HACK: Select "Resume Game" item by default if the game was paused.
      mMainMenu->SelectionUp();
   }

   mActive = true;
   mDone = false;
}

void UserInterface::HideMainMenu()
{
   mEntityManager.Reset();
   mMainMenu = nullptr;
   mActive = false;
}

UiItemId UserInterface::GetSelection() const
{
  return  mSelection;
}

MatchSettings UserInterface::GetMatchSettings() const
{
   return mSettings;
}

EntitySet UserInterface::GetEntities() const
{
   return mEntityManager.GetEntities();
}
