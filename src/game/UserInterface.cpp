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
      case SDLK_UP:
         mMainMenu->SelectionUp();
         break;
      case SDLK_DOWN:
         mMainMenu->SelectionDown();
         break;
      case SDLK_RETURN:
         mSelection = mMainMenu->GetSelection().id;
         mDone = true;
         break;
      case SDLK_BACKSPACE:
         mSelection = UserInterfaceItemId::MainMenu_ResumeGame;
         mDone = true;
         break;
      case SDLK_ESCAPE:
         mSelection = UserInterfaceItemId::MainMenu_Exit;
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

void UserInterface::ShowMainMenu(const bool game_paused)
{
   if (!mMainMenu)
   {
      mMainMenu = std::make_shared<MainMenu>();
      mMainMenu->AddMenuItem({ UserInterfaceItemId::MainMenu_ResumeGame, "Resume Game", true });
      mMainMenu->AddMenuItem({ UserInterfaceItemId::MainMenu_NewGame, "New Game", true });
      mMainMenu->AddMenuItem({ UserInterfaceItemId::MainMenu_Exit, "Exit", true });

      if (!game_paused) {
         mMainMenu->SelectionDown();
      }
   }

   mActive = true;
   mDone = false;
}

void UserInterface::HideMainMenu()
{
   mMainMenu = nullptr;
   mActive = false;
}

UserInterfaceItemId UserInterface::GetSelection() const
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
