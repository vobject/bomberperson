#ifndef MATCH_HPP
#define MATCH_HPP

#include "EventQueue.hpp"
#include "EntityManager.hpp"
#include "UserInterface.hpp"
#include "../utils/Utils.hpp"

#include <memory>

struct SDL_KeyboardEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;

class BomberPersonConfig;
class KeyboardInput;
class MouseInput;
//class KinectInput;

enum class ArenaType;
enum class PlayerType;

class Match
{
public:
   Match(BomberPersonConfig& cfg, const MatchSettings& settings);
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

   ArenaType ArenaTypeFromArenaId(ArenaId id);
   PlayerType PlayerTypeFromPlayerId(PlayerId id);

   BomberPersonConfig& mConfig;
   const MatchSettings mSettings;
   EventQueue mEventQueue;
   EntityManager mEntityManager;

   // The input devices get their own member variables for easier input update.
   std::pair<PlayerType, std::shared_ptr<KeyboardInput>> mKeyboard_1;
   std::pair<PlayerType, std::shared_ptr<KeyboardInput>> mKeyboard_2;
   std::pair<PlayerType, std::shared_ptr<MouseInput>> mMouse_1;
//   std::pair<PlayerType, std::shared_ptr<KinectInput>> mKinect_1;

   // We will clean up the mEntityManager of dead entities every X seconds.
   // This will keep track of the timing.
   int mCleanupIdleTime = 0_ms;

   bool mIsGamePaused = false;
   bool mIsGameOver = false;
};

#endif // MATCH_HPP
