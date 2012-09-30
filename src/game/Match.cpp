#include "Match.hpp"
#include "EventType.hpp"
#include "Player.hpp"
#include "../input/KeyboardInput.hpp"
#include "../input/MouseInput.hpp"
#include "../utils/Utils.hpp"
#include "../Options.hpp"

#include <SDL_events.h>

Match::Match(const MatchSettings& settings, const std::shared_ptr<Renderer>& renderer)
   : mSettings(settings)
   , mEntityManager(mEventQueue, renderer)
{
   // The arena is needed for the creation of the players.
   CreateArenaAndScoreboard();
   CreateInputDevicesAndPlayers();
}

Match::~Match()
{

}

void Match::Input(const SDL_KeyboardEvent& key)
{
   // TODO: Handle ESC key -> bring up menu and pause game.
   if ((SDL_KEYDOWN == key.type) && (SDLK_ESCAPE == key.keysym.sym))
   {
      mIsGamePaused = true;
      return;
   }

   if (SDL_KEYDOWN == key.type)
   {
      if (mKeyboard_1.second) {
         mKeyboard_1.second->Press(key.keysym.sym);
      }

      if (mKeyboard_2.second) {
         mKeyboard_2.second->Press(key.keysym.sym);
      }
   }
   else if (SDL_KEYUP == key.type)
   {
      if (mKeyboard_1.second) {
         mKeyboard_1.second->Release(key.keysym.sym);
      }

      if (mKeyboard_2.second) {
         mKeyboard_2.second->Release(key.keysym.sym);
      }
   }
}

void Match::Input(const SDL_MouseMotionEvent& motion)
{
   if (mMouse_1.second) {
      mMouse_1.second->Move({ motion.x, motion.y });
   }
}

void Match::Input(const SDL_MouseButtonEvent& button)
{
   if (SDL_MOUSEBUTTONDOWN == button.type)
   {
      if (mMouse_1.second) {
         mMouse_1.second->Press(button.button);
      }
   }
   else if (SDL_MOUSEBUTTONUP == button.type)
   {
      if (mMouse_1.second) {
         mMouse_1.second->Release(button.button);
      }
   }
}

void Match::Update(const int elapsed_time)
{
   CreateInputEvents();
   UpdateEntities(elapsed_time);
   mEventQueue.ProcessEvents();

   // Remove all entities that are marked as invalid during the last
   //  event processing cycle. This guarantees that an invalid entity
   //  will never be requested to update itself. Updating an invalid entity
   //  is dangerous because it may trigger new events.
   mEntityManager.Cleanup();

   // TODO: Query Scoreboard for game state.
//   mIsGameOver = (active_player_count <= 1) ? true : false;
}

bool Match::Pause() const
{
   return mIsGamePaused;
}

void Match::Resume()
{
   mIsGamePaused = false;
}

bool Match::GameOver() const
{
   return mIsGameOver;
}

EntitySet Match::GetEntities() const
{
   return mEntityManager.GetEntities();
}

void Match::CreateArenaAndScoreboard()
{
   const Point pos = { DefaultValue::ARENA_POS_X, DefaultValue::ARENA_POS_Y };
   const Size size = { DefaultValue::ARENA_WIDTH, DefaultValue::ARENA_HEIGHT };
   const Size borders = { DefaultValue::ARENA_BORDER_WIDTH, DefaultValue::ARENA_BORDER_HEIGHT };
   const auto cells_x = DefaultValue::ARENA_CELLS_X;
   const auto cells_y = DefaultValue::ARENA_CELLS_Y;
   const auto players = mSettings.players.size();

   // This is the very first event that must be created. The arena object is
   //  used internally to help create other ArenaObject-derived classes.
   mEventQueue.Add(std::make_shared<CreateArenaEvent>(pos, size, borders,
                                                      cells_x, cells_y, players));

   const Point sb_pos = { DefaultValue::SCOREBOARD_POS_X, DefaultValue::SCOREBOARD_POS_Y };
   const Size sb_size = { DefaultValue::SCOREBOARD_WIDTH, DefaultValue::SCOREBOARD_HEIGHT };

   mEventQueue.Add(std::make_shared<CreateScoreboardEvent>(sb_pos, sb_size));
}

void Match::CreateInputDevicesAndPlayers()
{
   for (const auto& p2i : mSettings.players)
   {
      const auto player_type = PlayerTypeFromPlayerId(p2i.first);

      if (InputId::Keyboard_1 == p2i.second)
      {
         mKeyboard_1 = { player_type,
                         std::make_shared<KeyboardInput>(SDLK_UP, SDLK_DOWN,
                                                         SDLK_LEFT, SDLK_RIGHT,
                                                         SDLK_SPACE, SDLK_LALT) };
      }

      if (InputId::Keyboard_2 == p2i.second)
      {
         mKeyboard_2 = { player_type,
                         std::make_shared<KeyboardInput>(SDLK_e, SDLK_d, SDLK_s,
                                                         SDLK_f, SDLK_a, SDLK_q) };
      }

      if (InputId::Mouse_1 == p2i.second)
      {
         const Point mouse_center(DefaultValue::SCREEN_WIDTH / 2,
                                  DefaultValue::SCREEN_HEIGHT / 2);

         mMouse_1 = { player_type,
                      std::make_shared<MouseInput>(mouse_center,
                                                   SDL_BUTTON_LEFT,
                                                   SDL_BUTTON_RIGHT) };
      }

      if (InputId::Kinect_1 == p2i.second)
      {
         throw "Kinect input device is not yet supported.";
      }

      mEventQueue.Add(std::make_shared<CreatePlayerEvent>(player_type));
   }
}

void Match::CreateInputEvents()
{
   mEventQueue.Add(std::make_shared<InputEvent>(mKeyboard_1.first,
                                                mKeyboard_1.second->TestUp(),
                                                mKeyboard_1.second->TestDown(),
                                                mKeyboard_1.second->TestLeft(),
                                                mKeyboard_1.second->TestRight(),
                                                mKeyboard_1.second->TestAction1(),
                                                mKeyboard_1.second->TestAction2()));

   mEventQueue.Add(std::make_shared<InputEvent>(mKeyboard_2.first,
                                                mKeyboard_2.second->TestUp(),
                                                mKeyboard_2.second->TestDown(),
                                                mKeyboard_2.second->TestLeft(),
                                                mKeyboard_2.second->TestRight(),
                                                mKeyboard_2.second->TestAction1(),
                                                mKeyboard_2.second->TestAction2()));

   mEventQueue.Add(std::make_shared<InputEvent>(mMouse_1.first,
                                                mMouse_1.second->TestUp(),
                                                mMouse_1.second->TestDown(),
                                                mMouse_1.second->TestLeft(),
                                                mMouse_1.second->TestRight(),
                                                mMouse_1.second->TestAction1(),
                                                mMouse_1.second->TestAction2()));
}

void Match::UpdateEntities(int elapsed_time)
{
   for (auto& ent : mEntityManager.GetEntities())
   {
      if (!ent->IsValid()) {
         continue;
      }
      ent->Update(elapsed_time);
   }

   // TODO: NOW is the time for collision detection.
   // TODO: Update all entities againg after collisions were detected.
}

PlayerType Match::PlayerTypeFromPlayerId(const PlayerId id)
{
   // Translate PlayerIds (which are option values) to EntityIds (used to
   //  identify Entities which may need resources and appear on the screen).

   switch (id)
   {
      case PlayerId::Player_1:
         return PlayerType::Player_1;
      case PlayerId::Player_2:
         return PlayerType::Player_2;
      case PlayerId::Player_3:
         return PlayerType::Player_3;
      case PlayerId::Player_4:
         return PlayerType::Player_4;
      default:
         throw "Unknown player id.";
   }
}
