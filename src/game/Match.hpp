#ifndef MATCH_HPP
#define MATCH_HPP

#include "EventQueue.hpp"
#include "EntityManager.hpp"
#include "UserInterface.hpp"
#include "../utils/Utils.hpp"

#include <memory>
#include <vector>

struct SDL_KeyboardEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;

class InputDevice;
class KeyboardInput;
class MouseInput;
//class KinectInput;

class Player;
class Scoreboard;
enum class PlayerType;

class Match
{
public:
   Match(const MatchSettings& settings);
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
   void UpdateEntities(int elapsed_time);

   void CreateInputDevices();
   void CreateInputEvents();
   PlayerType PlayerTypeFromPlayerId(PlayerId id);

   const MatchSettings mSettings;
   EventQueue mEventQueue;
   EntityManager mEntityManager;

   // The input devices get their own member variables for easier input update.
   std::pair<PlayerType, std::shared_ptr<KeyboardInput>> mKeyboard_1;
   std::pair<PlayerType, std::shared_ptr<KeyboardInput>> mKeyboard_2;
   std::pair<PlayerType, std::shared_ptr<MouseInput>> mMouse_1;
//   std::pair<PlayerType, std::shared_ptr<KinectInput>> mKinect_1;

//   // Yes, the stored pointers here are redundant:
//   // - a pointer to the player object exists inside the EntityManager
//   // - a pointer to the input device exists as member variable (see above)
//   // This data structure on the other side creates a mapping between
//   //  player and input device, so that we know what input device belongs
//   //  to which player.
//   std::vector<std::pair<std::shared_ptr<Player>,
//                         std::shared_ptr<InputDevice>>> mPlayerInputPair;

   // We will clean up the mEntityManager of dead entities every X seconds.
   // This will keep track of the timing.
   int mCleanupIdleTime = 0_ms;

   bool mIsGamePaused = false;
   bool mIsGameOver = false;
};

#endif // MATCH_HPP
