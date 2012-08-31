#include "Logic.hpp"
#include "MainMenu.hpp"
#include "Match.hpp"
#include "Background.hpp"
#include "ArenaGenerator.hpp"
#include "Arena.hpp"
#include "Cell.hpp"
#include "Player.hpp"
#include "../input/KeyboardInput.hpp"
#include "../input/MouseInput.hpp"
#include "../render/Renderer.hpp"
#include "../utils/Utils.hpp"
#include "../Options.hpp"

#include <SDL_events.h>

Logic::Logic(const std::shared_ptr<Renderer>& renderer)
   : mRenderer(renderer)
{
   ShowMainMenu();
}

Logic::~Logic()
{

}

void Logic::ProcessInput(const SDL_KeyboardEvent& key)
{
   switch (mCurrentState)
   {
      case GameState::MainMenu:
         ProcessInputMainMenuState(key);
         break;
      case GameState::Running:
         ProcessInputRunningState(key);
         break;
      case GameState::Exit:
         LOG(logERROR) << "ProcessInput: GameState is Exit.";
         break;
   }
}

void Logic::ProcessInput(const SDL_MouseMotionEvent& motion)
{
   if (GameState::Running != mCurrentState) {
      // Mouse input is currently only for moving players.
      return;
   }

   mMouse_1->Move({ motion.x, motion.y });
}

void Logic::ProcessInput(const SDL_MouseButtonEvent& button)
{
   if (GameState::Running != mCurrentState) {
      // Mouse input is currently only for moving players.
      return;
   }

   if (SDL_MOUSEBUTTONDOWN == button.type)
   {
      mMouse_1->Press(button.button);
   }
   else if (SDL_MOUSEBUTTONUP == button.type)
   {
      mMouse_1->Release(button.button);
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

   switch (mCurrentState)
   {
      case GameState::MainMenu:
         UpdateMainMenuState(elapsed_time);
         break;
      case GameState::Running:
         UpdateRunningState(elapsed_time);
         break;
      case GameState::Exit:
         break;
   }
}

void Logic::Render()
{
   switch (mCurrentState)
   {
      case GameState::MainMenu:
         mRenderer->PreRender();
         mRenderer->Render(mMainMenu);
         mRenderer->PostRender();
         break;
      case GameState::Running:
         mRenderer->PreRender();
         mRenderer->Render(mBackground);
         mRenderer->Render(mMatch);
         mRenderer->PostRender();
         break;
      case GameState::Exit:
         break;
   }
}

bool Logic::Done() const
{
   return (mCurrentState == GameState::Exit);
}

void Logic::ProcessInputMainMenuState(const SDL_KeyboardEvent& key)
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
         break;
      case SDLK_ESCAPE:
         mCurrentState = GameState::Exit;
         break;
      default:
         break;
   }
}

void Logic::ProcessInputRunningState(const SDL_KeyboardEvent& key)
{
   if (SDL_KEYDOWN == key.type)
   {
      if (SDLK_ESCAPE == key.keysym.sym)
      {
         // User pressed the ESC key while playing.
         ShowMainMenu();
         return;
      }

      mKeyboard_1->Press(key.keysym.sym);
      mKeyboard_2->Press(key.keysym.sym);
   }
   else if (SDL_KEYUP == key.type)
   {
      mKeyboard_1->Release(key.keysym.sym);
      mKeyboard_2->Release(key.keysym.sym);
   }
}

void Logic::UpdateMainMenuState(const int elapsed_time)
{
   if (!mMainMenu->HasChosen())
   {
      mMainMenu->Update(elapsed_time);
      return;
   }

   switch (mMainMenu->GetSelection())
   {
      case MainMenuItem::StartGame:
         ShowGame();
         break;
      case MainMenuItem::Exit:
         mCurrentState = GameState::Exit;
         break;
   }
}

void Logic::UpdateRunningState(const int elapsed_time)
{
   mMatch->Update(elapsed_time);
}

void Logic::ShowMainMenu()
{
   mMatch = nullptr;
   mFieldGen = nullptr;
   mBackground = nullptr;
   mMouse_1 = nullptr;
   mKeyboard_2 = nullptr;
   mKeyboard_1 = nullptr;

   mMainMenu = make_unique<MainMenu>("mainmenu");
   mMainMenu->SetSize({ DefaultSize::SCREEN_WIDTH, DefaultSize::SCREEN_HEIGHT });

   mCurrentState = GameState::MainMenu;
}

void Logic::ShowGame()
{
   mMainMenu = nullptr;

   mKeyboard_1 = std::make_shared<KeyboardInput>(SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_SPACE);
   mKeyboard_2 = std::make_shared<KeyboardInput>(SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_LCTRL);

   Point mouse_center(DefaultSize::SCREEN_WIDTH / 2, DefaultSize::SCREEN_HEIGHT / 2);
   mMouse_1 = std::make_shared<MouseInput>(mouse_center, SDL_BUTTON_LEFT);

   mBackground = std::make_shared<Background>("bg_arena_1");
   mBackground->SetSize({ DefaultSize::ARENA_BG_WIDTH, DefaultSize::ARENA_BG_HEIGHT });

   const std::vector<std::shared_ptr<Player>> players = {
      std::make_shared<Player>("player_1", mKeyboard_1)
    , std::make_shared<Player>("player_2", mMouse_1)
//    , std::make_shared<Player>("player_3")
//    , std::make_shared<Player>("player_4")
   };

   mFieldGen = std::make_shared<ArenaGenerator>();
   mFieldGen->SetArenaPosition({ DefaultSize::ARENA_POS_X, DefaultSize::ARENA_POS_Y });
   mFieldGen->SetArenaSize({ DefaultSize::ARENA_WIDTH, DefaultSize::ARENA_HEIGHT });
   auto arena = mFieldGen->GetDefaultArena(DefaultSize::ARENA_CELLS_X, DefaultSize::ARENA_CELLS_Y, players.size());

   const auto parent_cell_p1 = arena->GetCellFromCoordinates(DefaultSize::PLAYER_1_CELL_X, DefaultSize::PLAYER_1_CELL_Y);
   players[0]->SetParentCell(parent_cell_p1);
   players[0]->SetPosition(parent_cell_p1->GetPosition());
   players[0]->SetSize({ DefaultSize::PLAYER_WIDTH, DefaultSize::PLAYER_HEIGHT });

   const auto parent_cell_p2 = arena->GetCellFromCoordinates(DefaultSize::PLAYER_2_CELL_X, DefaultSize::PLAYER_2_CELL_Y);
   players[1]->SetParentCell(parent_cell_p2);
   players[1]->SetPosition(parent_cell_p2->GetPosition());
   players[1]->SetSize({ DefaultSize::PLAYER_WIDTH, DefaultSize::PLAYER_HEIGHT });

   mMatch = std::make_shared<Match>(arena, players);

   mCurrentState = GameState::Running;
}
