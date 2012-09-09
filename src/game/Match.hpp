#ifndef MATCH_HPP
#define MATCH_HPP

#include "EntityManager.hpp"

#include <memory>
#include <vector>

struct SDL_KeyboardEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;

class InputDevice;
class KeyboardInput;
class MouseInput;
//class KinectInput;
class Arena;
class Player;
class Scoreboard;
class Cell;

// Move these into UserInterface class -> where they are created.
enum class InputId
{
   Keyboard_1,
   Keyboard_2,
   Mouse_1,
   Kinect_1
};

// Move these into UserInterface class -> where they are created.
enum class PlayerId
{
   Player_1,
   Player_2,
   Player_3,
   Player_4
};

struct MatchSettings
{
   std::vector<std::pair<PlayerId, InputId>> players;
   // Level
   // Key config
};

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

//   bool IsOver() const;
   EntitySet GetEntities() const;

private:
   std::shared_ptr<Player> CreatePlayerFromPlayerId(PlayerId id, const std::shared_ptr<Arena> arena);
   std::shared_ptr<InputDevice> CreateInputFromInputId(InputId id);

   std::shared_ptr<Cell> GetCellFromObject(const std::shared_ptr<SceneObject>& obj) const;

   const MatchSettings mSettings;
   EntityManager mEntityManager;

   std::shared_ptr<Arena> mArena;

   // The input devices get their own member variables for easier input update.
   std::shared_ptr<KeyboardInput> mKeyboard_1;
   std::shared_ptr<KeyboardInput> mKeyboard_2;
   std::shared_ptr<MouseInput> mMouse_1;
//   std::shared_ptr<KinectInput> mKinect_1;

   // Yes, the stored pointers here are redundant:
   // - a pointer to the player object exists inside the EntityManager
   // - a pointer to the input device exists as member variable (see above)
   // This data structure on the other side creates a mapping between
   //  player and input device, so that we know what input device belongs
   //  to which player.
   std::vector<std::pair<std::shared_ptr<Player>,
                         std::shared_ptr<InputDevice>>> mPlayerInputPair;
//   std::shared_ptr<Scoreboard> mScoreboard;
};

#endif // MATCH_HPP
