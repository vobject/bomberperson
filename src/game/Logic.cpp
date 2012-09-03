#include "Logic.hpp"
#include "EntityId.hpp"
#include "MainMenu.hpp"
#include "Match.hpp"
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
#include <SDL_mixer.h>

//The music that will be played
Mix_Music *music = NULL;

Logic::Logic()
{
   ShowMainMenu();

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

void Logic::Render(const std::shared_ptr<Renderer>& renderer)
{
   switch (mCurrentState)
   {
      case GameState::MainMenu:
         renderer->PreRender();
         renderer->Render(mMainMenu);
         renderer->PostRender();
         break;
      case GameState::Running:
         renderer->PreRender();
         renderer->Render(mMatch);
         renderer->PostRender();
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
   if (!mMatch->IsOver())
   {
      mMatch->Update(elapsed_time);
      return;
   }

   ShowMainMenu();
}

void Logic::ShowMainMenu()
{
   mMatch = nullptr;
   mMouse_1 = nullptr;
   mKeyboard_2 = nullptr;
   mKeyboard_1 = nullptr;

   mMainMenu = make_unique<MainMenu>();
   mMainMenu->SetSize({ DefaultSize::SCREEN_WIDTH, DefaultSize::SCREEN_HEIGHT });

   mCurrentState = GameState::MainMenu;
}

void Logic::ShowGame()
{
   mMainMenu = nullptr;

   mKeyboard_1 = std::make_shared<KeyboardInput>(SDLK_UP, SDLK_DOWN, SDLK_LEFT, SDLK_RIGHT, SDLK_SPACE, SDLK_LALT);
   mKeyboard_2 = std::make_shared<KeyboardInput>(SDLK_e, SDLK_d, SDLK_s, SDLK_f, SDLK_q, SDLK_a);

   Point mouse_center(DefaultSize::SCREEN_WIDTH / 2, DefaultSize::SCREEN_HEIGHT / 2);
   mMouse_1 = std::make_shared<MouseInput>(mouse_center, SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT);

   const std::vector<std::shared_ptr<Player>> players = {
      std::make_shared<Player>(EntityId::Player_1, mKeyboard_1)
    , std::make_shared<Player>(EntityId::Player_2, mKeyboard_2)
    , std::make_shared<Player>(EntityId::Player_3, mMouse_1)
//    , std::make_shared<Player>(EntityId::Player_4, mKinect)
   };

   auto arena_gen = std::make_shared<ArenaGenerator>();
   arena_gen->SetArenaPosition({ DefaultSize::ARENA_POS_X, DefaultSize::ARENA_POS_Y });
   arena_gen->SetArenaSize({ DefaultSize::ARENA_WIDTH, DefaultSize::ARENA_HEIGHT });
   arena_gen->SetArenaBorderSize({ DefaultSize::ARENA_BORDER_WIDTH, DefaultSize::ARENA_BORDER_HEIGHT });
   auto arena = arena_gen->GetDefaultArena(DefaultSize::ARENA_CELLS_X, DefaultSize::ARENA_CELLS_Y, players.size());

   const auto parent_cell_p1 = arena->GetCellFromCoordinates(DefaultSize::PLAYER_1_CELL_X, DefaultSize::PLAYER_1_CELL_Y);
   players[0]->SetParentCell(parent_cell_p1);
   players[0]->SetPosition(parent_cell_p1->GetPosition());
   players[0]->SetSize({ DefaultSize::PLAYER_WIDTH, DefaultSize::PLAYER_HEIGHT });

   const auto parent_cell_p2 = arena->GetCellFromCoordinates(DefaultSize::PLAYER_2_CELL_X, DefaultSize::PLAYER_2_CELL_Y);
   players[1]->SetParentCell(parent_cell_p2);
   players[1]->SetPosition(parent_cell_p2->GetPosition());
   players[1]->SetSize({ DefaultSize::PLAYER_WIDTH, DefaultSize::PLAYER_HEIGHT });

   const auto parent_cell_p3 = arena->GetCellFromCoordinates(DefaultSize::PLAYER_3_CELL_X, DefaultSize::PLAYER_3_CELL_Y);
   players[2]->SetParentCell(parent_cell_p3);
   players[2]->SetPosition(parent_cell_p3->GetPosition());
   players[2]->SetSize({ DefaultSize::PLAYER_WIDTH, DefaultSize::PLAYER_HEIGHT });

   mMatch = std::make_shared<Match>(arena, players);
   mCurrentState = GameState::Running;
}

