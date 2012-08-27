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
#include <SDL_mixer.h>

//The music that will be played
Mix_Music *music = NULL;

Logic::Logic(const std::shared_ptr<Renderer>& renderer)
   : mRenderer(renderer)
{
   // Create a new game and start it.
   // This should usually just be done when the app cahnges from mainmenu-state
   //  or from the choose-match-options-state into the actual game-state.

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




   // SOUND TEST

   if(0 > SDL_Init(SDL_INIT_AUDIO)) {
      throw "Cannot init SDL audio subsystem.";
   }
   atexit(SDL_Quit);

   if(0 > Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)) {
      throw "Cannot init SDL audio subsystem.";
   }

//   //The sound effects that will be used
//   Mix_Chunk *scratch = NULL;
//   Mix_Chunk *high = NULL;
//   Mix_Chunk *med = NULL;
//   Mix_Chunk *low = NULL;

   //Load the music
   music = Mix_LoadMUS( "sound4.mp3" );

   //If there was a problem loading the music
   if( music == NULL ) {
      throw "Music load failed.";
   }
//   //Load the sound effects
//   scratch = Mix_LoadWAV( "scratch.wav" );
//   high = Mix_LoadWAV( "high.wav" );
//   med = Mix_LoadWAV( "medium.wav" );
//   low = Mix_LoadWAV( "low.wav" );
//   //If there was a problem loading the sound effects
//   if( ( scratch == NULL ) ||
//         ( high == NULL ) ||
//         ( med == NULL ) ||
//         ( low == NULL ) ) {
//      return false;
//   }

   //If there is no music playing
   if( Mix_PlayingMusic() == 0 )
   {
      //Play the music
      if( Mix_PlayMusic( music, -1 ) == -1 ) {
         throw "Music play failed.";
      }
   }
}

Logic::~Logic()
{
   //Free the music
   Mix_FreeMusic( music );

   //Quit SDL_mixer
   Mix_CloseAudio();
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
