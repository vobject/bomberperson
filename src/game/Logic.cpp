#include "Logic.hpp"
#include "UserInterface.hpp"
//#include "MainMenu.hpp"
#include "Match.hpp"
#include "SceneObject.hpp"
#include "../render/Renderer.hpp"
#include "../utils/Utils.hpp"

#include <SDL_events.h>

Logic::Logic()
{
   mUserInterface = std::make_shared<UserInterface>();
   mUserInterface->ShowMainMenu();
}

Logic::~Logic()
{

}

void Logic::ProcessInput(const SDL_KeyboardEvent& key)
{
   if (mUserInterface->IsActive())
   {
      mUserInterface->Input(key);
   }
   else
   {
      mMatch->Input(key);
   }

//   switch (mCurrentState)
//   {
//      case GameState::MainMenu:
//         ProcessInputMainMenuState(key);
//         break;
//      case GameState::Active:
//         ProcessInputRunningState(key);
//         break;
//      case GameState::Exit:
//         LOG(logERROR) << "ProcessInput: GameState is Exit.";
//         break;
//   }
}

void Logic::ProcessInput(const SDL_MouseMotionEvent& motion)
{
//   if (GameState::Active != mCurrentState) {
//      // Mouse input is currently only for moving players.
//      return;
//   }

//   mMouse_1->Move({ motion.x, motion.y });

   if (mUserInterface->IsActive())
   {
      mUserInterface->Input(motion);
   }
   else
   {
      mMatch->Input(motion);
   }
}

void Logic::ProcessInput(const SDL_MouseButtonEvent& button)
{
//   if (GameState::Active != mCurrentState) {
//      // Mouse input is currently only for moving players.
//      return;
//   }

//   if (SDL_MOUSEBUTTONDOWN == button.type)
//   {
//      mMouse_1->Press(button.button);
//   }
//   else if (SDL_MOUSEBUTTONUP == button.type)
//   {
//      mMouse_1->Release(button.button);
//   }

   if (mUserInterface->IsActive())
   {
      mUserInterface->Input(button);
   }
   else
   {
      mMatch->Input(button);
   }
}

//void Logic::ProcessInput(const kinex::Nui& kinect)
//{
////   // FIXME: This gets input data from one frame ago instead of the current one.
////   mKinectInput->Pressed(mPlayer->GetInput());
//}

void Logic::Update(const int app_time, const int elapsed_time)
{
   (void) app_time;

//   switch (mCurrentState)
//   {
//      case GameState::MainMenu:
//         UpdateMainMenuState(elapsed_time);
//         break;
//      case GameState::Active:
//         UpdateRunningState(elapsed_time);
//         break;
//      case GameState::Exit:
//         break;
//   }

   if (mUserInterface->IsActive())
   {
      if (!mUserInterface->IsDone())
      {
         mUserInterface->Update(elapsed_time);
      }
      else
      {
         switch (mUserInterface->GetSelection())
         {
            case UserInterfaceItem::MainMenu_NewGame:
               {
                  mUserInterface->HideMainMenu();

                  const auto settings = mUserInterface->GetMatchSettings();
                  mMatch = std::make_shared<Match>(settings);
               }
               break;
            case UserInterfaceItem::MainMenu_Exit:
               mDone = true;
               break;
            default:
               LOG(logERROR) << "ProcessInput: Unknown menu selection.";
               break;
         }
      }
   }
   else
   {
      if (mMatch->GameOver())
      {
         mMatch = nullptr;
         mUserInterface->ShowMainMenu();
      }
      else
      {
         mMatch->Update(elapsed_time);

         // TODO: Why not put this into Match::Update().
         for (auto& ent : mMatch->GetEntities())
         {
            if (!ent->IsAlive()) {
               continue;
            }
            ent->Update(elapsed_time);
         }

         // TODO: NOW is the time for collision detection.
         // TODO: Update all entities againg after collisions were detected.
      }
   }
}

void Logic::Render(const std::shared_ptr<Renderer>& renderer)
{
//   switch (mCurrentState)
//   {
//      case GameState::MainMenu:
//         renderer->PreRender();
//         renderer->Render(mMainMenu);
//         renderer->PostRender();
//         break;
//      case GameState::Active:
//         renderer->PreRender();
//         renderer->Render(mMatch);
//         renderer->PostRender();
//         break;
//      case GameState::Exit:
//         break;
//   }


   renderer->PreRender();

   if (mUserInterface->IsActive())
   {
      for (const auto& ent : mUserInterface->GetEntities())
      {
         if (!ent->IsAlive()) {
            continue;
         }
         renderer->Render(ent);
      }
   }
   else
   {
      for (const auto& ent : mMatch->GetEntities())
      {
         if (!ent->IsAlive()) {
            continue;
         }
         renderer->Render(ent);
      }
   }

   renderer->PostRender();
}

bool Logic::Done() const
{
   return mDone;
}

//void Logic::ProcessInputMainMenuState(const SDL_KeyboardEvent& key)
//{
//   if (SDL_KEYDOWN != key.type) {
//      return;
//   }

//   switch (key.keysym.sym)
//   {
//      case SDLK_UP:
//         mMainMenu->SelectionUp();
//         break;
//      case SDLK_DOWN:
//         mMainMenu->SelectionDown();
//         break;
//      case SDLK_RETURN:
//         mMainMenu->Choose();
//         break;
//      case SDLK_ESCAPE:
//         mCurrentState = GameState::Exit;
//         break;
//      default:
//         break;
//   }
//}

//void Logic::ProcessInputRunningState(const SDL_KeyboardEvent& key)
//{
//   if (SDL_KEYDOWN == key.type)
//   {
//      if (SDLK_ESCAPE == key.keysym.sym)
//      {
//         // User pressed the ESC key while playing.
//         ShowMainMenu();
//         return;
//      }

//      mKeyboard_1->Press(key.keysym.sym);
//      mKeyboard_2->Press(key.keysym.sym);
//   }
//   else if (SDL_KEYUP == key.type)
//   {
//      mKeyboard_1->Release(key.keysym.sym);
//      mKeyboard_2->Release(key.keysym.sym);
//   }
//}

//void Logic::UpdateMainMenuState(const int elapsed_time)
//{
//   if (!mMainMenu->HasChosen())
//   {
//      mMainMenu->Update(elapsed_time);
//      return;
//   }

//   switch (mMainMenu->GetSelection())
//   {
//      case MainMenuItem::StartGame:
//         ShowGame();
//         break;
//      case MainMenuItem::Exit:
//         mCurrentState = GameState::Exit;
//         break;
//   }
//}

//void Logic::UpdateRunningState(const int elapsed_time)
//{
//   for (auto& player : mEntityManager.GetPlayers())
//   {
//      switch (player->GetId())
//      {
//         case EntityId::Player_1:
//            player->SetInputCommands(mKeyboard_1->GetCommands());
//            break;
//         case EntityId::Player_2:
//            player->SetInputCommands(mKeyboard_2->GetCommands());
//            break;
//         case EntityId::Player_3:
//            player->SetInputCommands(mMouse_1->GetCommands());
//            break;
////         case EntityId::Player_4:
////            player->SetInputCommands(mKinect->GetCommands());
////            break;
//         default:
//            LOG(logERROR) << "Player has invalid EntityId!";
//            break;
//      }
//   }

//   if (!mMatch->IsOver())
//   {
//      mMatch->Update(elapsed_time);
//      return;
//   }

//   ShowMainMenu();
//}

//void Logic::ShowMainMenu()
//{
//   mMatch = nullptr;
//   mMouse_1 = nullptr;
//   mKeyboard_2 = nullptr;
//   mKeyboard_1 = nullptr;

//   mMainMenu = std::make_shared<MainMenu>();
//   mMainMenu->SetSize({ DefaultSize::SCREEN_WIDTH, DefaultSize::SCREEN_HEIGHT });

//   mCurrentState = GameState::MainMenu;
//}

//void Logic::ShowGame()
//{
//   mMainMenu = nullptr;

//   mKeyboard_1 = std::make_shared<KeyboardInput>(SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_SPACE, SDLK_LALT);
//   mKeyboard_2 = std::make_shared<KeyboardInput>(SDLK_e, SDLK_d, SDLK_s, SDLK_f, SDLK_q, SDLK_a);

//   Point mouse_center(DefaultSize::SCREEN_WIDTH / 2, DefaultSize::SCREEN_HEIGHT / 2);
//   mMouse_1 = std::make_shared<MouseInput>(mouse_center, SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT);

//   auto arena_gen = std::make_shared<ArenaGenerator>();
//   arena_gen->SetArenaPosition({ DefaultSize::ARENA_POS_X, DefaultSize::ARENA_POS_Y });
//   arena_gen->SetArenaSize({ DefaultSize::ARENA_WIDTH, DefaultSize::ARENA_HEIGHT });
//   arena_gen->SetArenaBorderSize({ DefaultSize::ARENA_BORDER_WIDTH, DefaultSize::ARENA_BORDER_HEIGHT });
//   auto arena = arena_gen->GetDefaultArena(DefaultSize::ARENA_CELLS_X, DefaultSize::ARENA_CELLS_Y, 3);

//   mEntityManager.CreatePlayers(3, arena);

//   mMatch = std::make_shared<Match>(arena, mEntityManager.GetPlayers());
//   mCurrentState = GameState::Active;
//}
