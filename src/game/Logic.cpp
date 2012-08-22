#include "Logic.hpp"
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
   // Create a new game and start it.
   // This should usually just be done when the app cahnges from mainmenu-state
   //  or from the choose-match-options-state into the actual game-state.

   mBackground = std::make_shared<Background>(DefaultOptions::ARENA_BG_ID);
   mBackground->SetSize({ DefaultOptions::ARENA_BG_WIDTH,
                          DefaultOptions::ARENA_BG_HEIGHT });
   mBackground->SetBorderSize({ DefaultOptions::ARENA_BG_BORDER_WIDTH,
                                DefaultOptions::ARENA_BG_BORDER_HEIGHT });

   const std::vector<std::shared_ptr<Player>> players = {
      std::make_shared<Player>(DefaultOptions::PLAYER_ID_1)
    , std::make_shared<Player>(DefaultOptions::PLAYER_ID_2)
   };

   mFieldGen = std::make_shared<ArenaGenerator>();
   mFieldGen->SetArenaPosition({ mBackground->GetBorderSize().Width,
                                 mBackground->GetBorderSize().Height });
   mFieldGen->SetArenaSize({ DefaultOptions::ARENA_WIDTH,
                             DefaultOptions::ARENA_HEIGHT });
   auto arena = mFieldGen->GetDefaultArena(DefaultOptions::ARENA_CELLS_X,
                                           DefaultOptions::ARENA_CELLS_Y,
                                           players.size());

   const auto input_p1 = std::make_shared<KeyboardInput>(SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_SPACE);
   const auto parent_cell_p1 = arena->GetCellFromCoordinates(DefaultOptions::PLAYER_1_CELL_X,
                                                             DefaultOptions::PLAYER_1_CELL_Y);
   players[0]->SetInputDevice(input_p1);
   players[0]->SetParentCell(parent_cell_p1);
   players[0]->SetPosition(parent_cell_p1->GetPosition());
   players[0]->SetSize({ DefaultOptions::PLAYER_WIDTH,
                         DefaultOptions::PLAYER_HEIGHT });

   const auto input_p2 = std::make_shared<KeyboardInput>(SDLK_w, SDLK_s, SDLK_a, SDLK_d, SDLK_LCTRL);
   const auto parent_cell_p2 = arena->GetCellFromCoordinates(DefaultOptions::PLAYER_2_CELL_X,
                                                             DefaultOptions::PLAYER_2_CELL_Y);
   players[1]->SetInputDevice(input_p2);
   players[1]->SetParentCell(parent_cell_p2);
   players[1]->SetPosition(parent_cell_p2->GetPosition());
   players[1]->SetSize({ DefaultOptions::PLAYER_WIDTH,
                         DefaultOptions::PLAYER_HEIGHT });

   mMatch = std::make_shared<Match>(arena, players);
}

Logic::~Logic()
{

}

void Logic::ProcessInput(const SDL_KeyboardEvent& ev)
{
   // TODO: Find out if non-Up/Down Handling is a better choice: GetKeystate()

   const auto players = mMatch->GetPlayers();

   if (SDL_KEYDOWN == ev.type)
   {
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
}

//void Logic::ProcessInput(const kinex::Nui& kinect)
//{
////   // FIXME: This gets input data from one frame ago instead of the current one.
////   mKinectInput->Pressed(mPlayer->GetInput());
//}

void Logic::Update(const int app_time, const int elapsed_time)
{
   mMatch->Update(elapsed_time);
}

void Logic::Render()
{
   mRenderer->PreRender();
   mRenderer->Render(mBackground);
   mRenderer->Render(mMatch);
   mRenderer->PostRender();
}
