#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <memory>
#include <list>

struct SDL_KeyboardEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;

class Audio;
class UserInterface;
class Renderer;
class ArenaGenerator;
class KeyboardInput;
class MouseInput;
class Match;

//enum class GameState
//{
//   MainMenu,
////   GameSetup,
//   Active,
////   Options,
////   Credits,
//   Exit
//};

class Logic
{
public:
   Logic();
   ~Logic();

   Logic(const Logic&) = delete;
   Logic& operator=(const Logic&) = delete;

   void ProcessInput(const SDL_KeyboardEvent& key);
   void ProcessInput(const SDL_MouseMotionEvent& motion);
   void ProcessInput(const SDL_MouseButtonEvent& button);
//   void ProcessInput(const kinex::Nui& kinect);

   void Update(int app_time, int elapsed_time);
   void Play(const std::shared_ptr<Audio>& audio);
   void Render(const std::shared_ptr<Renderer>& renderer);

   bool Done() const;

private:
   void NewGame();
//   void ReturnToGame();
//   void GameOver();

//   void ProcessInputMainMenuState(const SDL_KeyboardEvent& ev);
//   void ProcessInputRunningState(const SDL_KeyboardEvent& ev);

//   void UpdateMainMenuState(int elapsed_time);
//   void UpdateRunningState(int elapsed_time);

//   void ShowMainMenu();
//   void ShowGame();

   // TODO:
   // class MatchSettings{ nplayers, player:inputdev };
   // std::shared_ptr<UserInterface> mUserInterface;
   // std::shared_ptr<Match> mMatch;
   // mMatch = std::make_shared<Match>(mUserInterface->GetMatchSettings());

//   GameState mCurrentState = GameState::MainMenu;

//   std::shared_ptr<MainMenu> mMainMenu;

   bool mDone = false;
   std::shared_ptr<UserInterface> mUserInterface;
   std::shared_ptr<Match> mMatch;
};

#endif // LOGIC_HPP
