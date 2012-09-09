#include "Match.hpp"
#include "Arena.hpp"
#include "ArenaGenerator.hpp"
#include "Cell.hpp"
#include "Player.hpp"
#include "Scoreboard.hpp"
#include "../input/KeyboardInput.hpp"
#include "../input/MouseInput.hpp"
#include "../utils/Utils.hpp"
#include "../Options.hpp"

Match::Match(const MatchSettings& settings)
   : mSettings(settings)
{
   // The arena is needed for the creation of the players.
   mArena = mEntityManager.CreateArena(mSettings.players.size());
   auto scoreboard = mEntityManager.CreateScoreboard();

   for (const auto& p2i : mSettings.players)
   {
      const auto player = CreatePlayerFromPlayerId(p2i.first, mArena);
      const auto input = CreateInputFromInputId(p2i.second);

      scoreboard->KeepTrackOf(player);
      mPlayerInputPair.push_back({ player, input });
   }
}

Match::~Match()
{

}

void Match::Input(const SDL_KeyboardEvent& key)
{
   // TODO: Handle ESC key -> bring up menu and pause game.

   if (SDL_KEYDOWN == key.type)
   {
      mKeyboard_1->Press(key.keysym.sym);
      mKeyboard_2->Press(key.keysym.sym);
   }
   else if (SDL_KEYUP == key.type)
   {
      mKeyboard_1->Release(key.keysym.sym);
      mKeyboard_2->Release(key.keysym.sym);
   }
}

void Match::Input(const SDL_MouseMotionEvent& motion)
{
   mMouse_1->Move({ motion.x, motion.y });
}

void Match::Input(const SDL_MouseButtonEvent& button)
{
   if (SDL_MOUSEBUTTONDOWN == button.type)
   {
      mMouse_1->Press(button.button);
   }
   else if (SDL_MOUSEBUTTONUP == button.type)
   {
      mMouse_1->Release(button.button);
   }
}

void Match::Update(const int elapsed_time)
{
   mCleanupIdleTime += elapsed_time;
   if (mCleanupIdleTime >= 1000_ms)
   {
      // Remove dead sprites every second.
      mEntityManager.Cleanup();
      mCleanupIdleTime = 0_ms;
   }

   int active_player_count = 0;
   for (const auto& player_input : mPlayerInputPair)
   {
      player_input.first->SetInputCommands(player_input.second->GetCommands());
      player_input.first->SetParentCell(GetCellFromObject(player_input.first));

      if (player_input.first->IsAlive()) {
         active_player_count++;
      }
   }

   mIsGameOver = (active_player_count <= 1) ? true : false;
}

bool Match::GameOver() const
{
   return mIsGameOver;
}

EntitySet Match::GetEntities() const
{
   return mEntityManager.GetEntities();
}

std::shared_ptr<Player> Match::CreatePlayerFromPlayerId(
   const PlayerId id,
   const std::shared_ptr<Arena> arena
)
{
   // Translate PlayerIds (which are option values) to EntityIds (used to
   //  identify Entities which may need resources and appear on the screen).

   switch (id)
   {
      case PlayerId::Player_1:
         return mEntityManager.CreatePlayer(EntityId::Player_1, arena);
      case PlayerId::Player_2:
         return mEntityManager.CreatePlayer(EntityId::Player_2, arena);
      case PlayerId::Player_3:
         return mEntityManager.CreatePlayer(EntityId::Player_3, arena);
      case PlayerId::Player_4:
         return mEntityManager.CreatePlayer(EntityId::Player_4, arena);
   }
   return nullptr;
}

std::shared_ptr<InputDevice> Match::CreateInputFromInputId(const InputId id)
{
   // Create actual InputDevice from InputIds (defined in the option menus).

   switch (id)
   {
      case InputId::Keyboard_1:
      {
         mKeyboard_1 = std::make_shared<KeyboardInput>(SDLK_UP, SDLK_DOWN,
                                                       SDLK_LEFT, SDLK_RIGHT,
                                                       SDLK_SPACE, SDLK_LALT);
         return mKeyboard_1;
      }
      case InputId::Keyboard_2:
      {
         mKeyboard_2 = std::make_shared<KeyboardInput>(SDLK_e, SDLK_d, SDLK_s,
                                                       SDLK_f, SDLK_q, SDLK_a);
         return mKeyboard_2;
      }
      case InputId::Mouse_1:
      {
         const Point mouse_center(DefaultSize::SCREEN_WIDTH / 2,
                                  DefaultSize::SCREEN_HEIGHT / 2);
         mMouse_1 = std::make_shared<MouseInput>(mouse_center, SDL_BUTTON_LEFT,
                                                 SDL_BUTTON_RIGHT);
         return mMouse_1;
      }
      case InputId::Kinect_1:
      {
         LOG(logERROR) << "Kinect input device is not yet supported.";
         return nullptr;
      }
   }
   return nullptr;
}

std::shared_ptr<Cell> Match::GetCellFromObject(
   const std::shared_ptr<SceneObject>& obj
) const
{
   // The objects position is its center.
   const Point pos = { obj->GetPosition().X + (obj->GetSize().Width / 2),
                       obj->GetPosition().Y + (obj->GetSize().Height / 2) };
   return mArena->GetCellFromPosition(pos);
}
