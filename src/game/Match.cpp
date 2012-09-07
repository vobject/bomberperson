#include "Match.hpp"

#include "../input/KeyboardInput.hpp"
#include "../input/MouseInput.hpp"

#include "Arena.hpp"
#include "ArenaGenerator.hpp"

#include "Scoreboard.hpp"
#include "Cell.hpp"
#include "Player.hpp"
#include "../utils/Utils.hpp"
#include "../Options.hpp"

Match::Match(const MatchSettings& settings)
   : mSettings(settings)
//   : mArena(arena)
//   , mPlayers(players)
//   , mScoreboard(std::make_shared<Scoreboard>())
{
//   mScoreboard->SetPosition({ DefaultSize::SCOREBOARD_POS_X,
//                              DefaultSize::SCOREBOARD_POS_Y });
//   mScoreboard->SetSize({ DefaultSize::SCOREBOARD_WIDTH,
//                          DefaultSize::SCOREBOARD_HEIGHT });

//   mScoreboard->KeepTrackOf(mArena);
//   mScoreboard->KeepTrackOf(mPlayers);

   // The arena is needed for the creation of the players.
   auto arena = mEntityManager.CreateArena(mSettings.players.size());

   for (const auto& p2i : mSettings.players)
   {
      mPlayerInputPair.push_back({ CreatePlayerFromPlayerId(p2i.first, arena),
                                   CreateInputFromInputId(p2i.second) });
   }
}

Match::~Match()
{

}

void Match::Input(const SDL_KeyboardEvent& key)
{
   // TODO: Handle ESC key -> bring up menu.

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
//   mArena->Update(elapsed_time);

//   auto player = std::begin(mPlayers);
//   while (player != std::end(mPlayers))
//   {
//      if ((*player)->IsAlive())
//      {
//         (*player)->Update(elapsed_time);
//         (*player)->SetParentCell(GetCellFromObject(*player));
//         player++;
//      }
//      else
//      {
//         player = mPlayers.erase(player);
//      }
//   }

//   mScoreboard->Update(elapsed_time);

   (void) elapsed_time; // TODO: Keep track of how long the game went on.

   UpdatePlayerInputCommands();
}

//std::shared_ptr<Arena> Match::GetArena() const
//{
//   return mArena;
//}

//std::vector<std::shared_ptr<Player>> Match::GetPlayers() const
//{
//   return mPlayers;
//}

//std::shared_ptr<Scoreboard> Match::GetScoreboard() const
//{
//   return mScoreboard;
//}

//bool Match::IsOver() const
//{
//   return (mPlayers.size() <= 1);
//}

//std::shared_ptr<Cell> Match::GetCellFromObject(
//   const std::shared_ptr<SceneObject>& obj
//) const
//{
//   const Point pos = { obj->GetPosition().X + (obj->GetSize().Width / 2),
//                       obj->GetPosition().Y + (obj->GetSize().Height / 2) };
//   return mArena->GetCellFromPosition(pos);
//}

std::vector<std::shared_ptr<SceneObject>> Match::GetEntities() const
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

void Match::UpdatePlayerInputCommands() const
{
   for (const auto& player_input : mPlayerInputPair)
   {
      player_input.first->SetInputCommands(player_input.second->GetCommands());
   }
}
