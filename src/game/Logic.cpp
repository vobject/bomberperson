#include "Logic.hpp"
#include "MainMenu.hpp"
#include "Match.hpp"
#include "Background.hpp"
#include "ArenaGenerator.hpp"
#include "Arena.hpp"
#include "Cell.hpp"
#include "Player.hpp"
#include "../input/KeyboardInput.hpp"
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

void Logic::ProcessInput(const SDL_KeyboardEvent& ev)
{
   switch (mCurrentState)
   {
      case GameState::MainMenu:
      {
         if (SDL_KEYDOWN != ev.type) {
            return;
         }

         if (SDLK_UP == ev.keysym.sym)
         {
            mMainMenu->SelectionUp();
         }
         else if (SDLK_DOWN == ev.keysym.sym)
         {
            mMainMenu->SelectionDown();
         }
         else if (SDLK_RETURN == ev.keysym.sym)
         {
            mMainMenu->Choose();
         }
         else if(SDLK_ESCAPE == ev.keysym.sym)
         {
            mCurrentState = GameState::Exit;
         }
         break;
      }
      case GameState::Running:
      {
         const auto players = mMatch->GetPlayers();

         if (SDL_KEYDOWN == ev.type)
         {
            if (SDLK_ESCAPE == ev.keysym.sym)
            {
               ShowMainMenu();
               return;
            }

            for (auto& player : players)
            {
               player->GetInputDevice()->Press(ev.keysym.sym);
            }
         }
         else if (SDL_KEYUP == ev.type)
         {
            for (auto& player : players)
            {
               player->GetInputDevice()->Release(ev.keysym.sym);
            }
         }
         break;
      }
      case GameState::Exit:
         LOG(logERROR) << "Logic: GameState is Exit.";
         break;
      default:
         LOG(logERROR) << "Logic: Unknown GameState value.";
         break;
   }
}

//void Logic::ProcessInput(const kinex::Nui& kinect)
//{
////   // FIXME: This gets input data from one frame ago instead of the current one.
////   mKinectInput->Pressed(mPlayer->GetInput());
//}

void Logic::Update(const int app_time, const int elapsed_time)
{
   switch (mCurrentState)
   {
      case GameState::MainMenu:
      {
         if (mMainMenu->HasChosen())
         {
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
         else
         {
            mMainMenu->Update(elapsed_time);
         }
         break;
      }
      case GameState::Running:
         mMatch->Update(elapsed_time);
         break;
      case GameState::Exit:
         break;
   }
}

void Logic::Render()
{
   mRenderer->PreRender();

   switch (mCurrentState)
   {
      case GameState::MainMenu:
         mRenderer->Render(mMainMenu);
         break;
      case GameState::Running:
         mRenderer->Render(mBackground);
         mRenderer->Render(mMatch);
         break;
      case GameState::Exit:
         break;
   }

   mRenderer->PostRender();
}

bool Logic::Done() const
{
   return (mCurrentState == GameState::Exit);
}

void Logic::ShowMainMenu()
{
   mMatch = nullptr;
   mFieldGen = nullptr;
   mBackground = nullptr;

   mMainMenu = make_unique<MainMenu>("mainmenu");
   mMainMenu->SetSize({ DefaultSize::SCREEN_WIDTH, DefaultSize::SCREEN_HEIGHT });

   mCurrentState = GameState::MainMenu;
}

void Logic::ShowGame()
{
   mMainMenu = nullptr;

   mBackground = std::make_shared<Background>("bg_arena_1");
   mBackground->SetSize({ DefaultSize::ARENA_BG_WIDTH, DefaultSize::ARENA_BG_HEIGHT });

   const std::vector<std::shared_ptr<Player>> players = {
      std::make_shared<Player>("player_1")
    , std::make_shared<Player>("player_2")
//    , std::make_shared<Player>("player_3")
//    , std::make_shared<Player>("player_4")
   };

   mFieldGen = std::make_shared<ArenaGenerator>();
   mFieldGen->SetArenaPosition({ DefaultSize::ARENA_POS_X, DefaultSize::ARENA_POS_Y });
   mFieldGen->SetArenaSize({ DefaultSize::ARENA_WIDTH, DefaultSize::ARENA_HEIGHT });
   auto arena = mFieldGen->GetDefaultArena(DefaultSize::ARENA_CELLS_X, DefaultSize::ARENA_CELLS_Y, players.size());

   const auto input_p1 = std::make_shared<KeyboardInput>(SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_SPACE);
   const auto parent_cell_p1 = arena->GetCellFromCoordinates(DefaultSize::PLAYER_1_CELL_X, DefaultSize::PLAYER_1_CELL_Y);
   players[0]->SetInputDevice(input_p1);
   players[0]->SetParentCell(parent_cell_p1);
   players[0]->SetPosition(parent_cell_p1->GetPosition());
   players[0]->SetSize({ DefaultSize::PLAYER_WIDTH, DefaultSize::PLAYER_HEIGHT });

   const auto input_p2 = std::make_shared<KeyboardInput>(SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_LCTRL);
   const auto parent_cell_p2 = arena->GetCellFromCoordinates(DefaultSize::PLAYER_2_CELL_X, DefaultSize::PLAYER_2_CELL_Y);
   players[1]->SetInputDevice(input_p2);
   players[1]->SetParentCell(parent_cell_p2);
   players[1]->SetPosition(parent_cell_p2->GetPosition());
   players[1]->SetSize({ DefaultSize::PLAYER_WIDTH, DefaultSize::PLAYER_HEIGHT });

   mMatch = std::make_shared<Match>(arena, players);

   mCurrentState = GameState::Running;
}
