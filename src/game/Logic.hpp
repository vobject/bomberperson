#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <memory>
#include <list>

struct SDL_KeyboardEvent;

class MainMenu;
class Renderer;
class Background;
class ArenaGenerator;
class Match;

enum class GameState
{
   MainMenu,
//   GameSetup,
   Running,
//   Options,
//   Credits,
   Exit
};

class Logic
{
public:
   Logic(const std::shared_ptr<Renderer>& renderer);
   ~Logic();

   Logic(const Logic&) = delete;
   Logic& operator=(const Logic&) = delete;

   void ProcessInput(const SDL_KeyboardEvent& ev);
//   void ProcessInput(const kinex::Nui& kinect);

   void Update(int app_time, int elapsed_time);
   void Render();

   bool Done() const;

private:
   void ShowMainMenu();
   void ShowGame();

   GameState mCurrentState = GameState::MainMenu;

   std::shared_ptr<MainMenu> mMainMenu;
   std::shared_ptr<Renderer> mRenderer;
   std::shared_ptr<Background> mBackground;
   std::shared_ptr<ArenaGenerator> mFieldGen;
   std::shared_ptr<Match> mMatch;
};

#endif // LOGIC_HPP
