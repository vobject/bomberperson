#ifndef MATCH_HPP
#define MATCH_HPP

#include "EventQueue.hpp"
#include "EntityManager.hpp"
#include "UserInterface.hpp"
#include "../utils/Utils.hpp"

#include <memory>

class Renderer;

struct SDL_KeyboardEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;

class KeyboardInput;
class MouseInput;
//class KinectInput;

enum class PlayerType;

class Match
{
public:
   Match(const MatchSettings& settings, const std::shared_ptr<Renderer>& renderer);
   ~Match();

   Match(const Match&) = delete;
   Match& operator=(const Match&) = delete;

   void Input(const SDL_KeyboardEvent& key);
   void Input(const SDL_MouseMotionEvent& motion);
   void Input(const SDL_MouseButtonEvent& button);
//   void Input(const Nui& kinect);

   void Update(int elapsed_time);

   bool Pause() const;
   void Resume();

   bool GameOver() const;

   EntitySet GetEntities() const;

private:
   void CreateArenaAndScoreboard();
   void CreateInputDevicesAndPlayers();

   void CreateInputEvents();
   void UpdateEntities(int elapsed_time);

   PlayerType PlayerTypeFromPlayerId(PlayerId id);

   const MatchSettings mSettings;
   EventQueue mEventQueue;
   EntityManager mEntityManager;

   // The input devices get their own member variables for easier input update.
   std::pair<PlayerType, std::shared_ptr<KeyboardInput>> mKeyboard_1;
   std::pair<PlayerType, std::shared_ptr<KeyboardInput>> mKeyboard_2;
   std::pair<PlayerType, std::shared_ptr<MouseInput>> mMouse_1;
//   std::pair<PlayerType, std::shared_ptr<KinectInput>> mKinect_1;

   bool mIsGamePaused = false;
   bool mIsGameOver = false;
};

#endif // MATCH_HPP
