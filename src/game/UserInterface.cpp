#include "UserInterface.hpp"
#include "../BomberPersonConfig.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"
#include "../input/KeyboardInput.hpp"

#include <SDL_events.h>

UserInterface::UserInterface(BomberPersonConfig& cfg)
   : mConfig(cfg)
   , mEntityManager(mConfig, mEventQueue)
{
   mEventQueue.Register(this);

   mKeyboard = std::make_shared<KeyboardInput>(SDLK_UP, SDLK_DOWN,
                                               SDLK_LEFT, SDLK_RIGHT,
                                               SDLK_RETURN, SDLK_ESCAPE);

   // Create all menu types available and keep them alive
   // as long as the UserInterface class exists.

   mEventQueue.Add(std::make_shared<CreateMainMenuEvent>(
      Point{0, 0},
      mConfig.GetResolution())
   );

   mSettings = {
      ArenaId::Arena_1,
      {
         { PlayerId::Player_1, InputId::Keyboard_1 },
         { PlayerId::Player_2, InputId::Keyboard_2 },
         { PlayerId::Player_3, InputId::Mouse_1 }
      }
   };
}

UserInterface::~UserInterface()
{
   mEventQueue.UnRegister(this);
}

void UserInterface::Input(const SDL_KeyboardEvent& key)
{
   if (SDL_KEYDOWN == key.type) {
      mKeyboard->Press(key.keysym.sym);
   }
}

void UserInterface::Input(const SDL_MouseMotionEvent& motion)
{
   (void) motion;
}

void UserInterface::Input(const SDL_MouseButtonEvent& button)
{
   (void) button;
}

void UserInterface::Update(const int elapsed_time)
{
   CreateInputEvents();
   UpdateEntities(elapsed_time);
   mEventQueue.ProcessEvents();
}

void UserInterface::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::MainMenuAction:
         OnMainMenuAction(dynamic_cast<const MainMenuActionEvent&>(event));
         break;
      default:
         break;
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

MainMenuItem UserInterface::GetSelection() const
{
  return  mSelection;
}

void UserInterface::ShowMainMenu(const bool game_paused)
{
   if (game_paused)
   {
      mEventQueue.Add(std::make_shared<MainMenuEnableEvent>(MainMenuItem::ResumeGame, true));

//      // HACK: Select "Resume Game" item by default if the game was paused.
//      mEventQueue.Add(std::make_shared<MenuInputEvent>(MenuType::MainMenu,
//                                                       true, false,
//                                                       false, false,
//                                                       false, false));
   }

   mActive = true;
   mDone = false;
}

void UserInterface::HideMainMenu()
{
//   mEntityManager.Reset();
   mActive = false;
}

MatchSettings UserInterface::GetMatchSettings() const
{
   return mSettings;
}

EntitySet UserInterface::GetEntities() const
{
   return mEntityManager.GetEntities();
}

void UserInterface::OnMainMenuAction(const MainMenuActionEvent& event)
{
   mSelection = event.GetItem();
   mDone = true;
}

void UserInterface::CreateInputEvents()
{
   mEventQueue.Add(std::make_shared<MenuInputEvent>(MenuType::MainMenu,
                                                    mKeyboard->TestUp(),
                                                    mKeyboard->TestDown(),
                                                    mKeyboard->TestLeft(),
                                                    mKeyboard->TestRight(),
                                                    mKeyboard->TestAction1(),
                                                    mKeyboard->TestAction2()));
   mKeyboard->Reset();
}

void UserInterface::UpdateEntities(const int elapsed_time)
{
   for (auto& ent : mEntityManager.GetEntities())
   {
      if (!ent->IsValid()) {
         continue;
      }
      ent->Update(elapsed_time);
   }
}
