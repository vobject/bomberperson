#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <memory>

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

class Logic
{
public:
   Logic();
   ~Logic();

   Logic(const Logic&) = delete;
   Logic& operator=(const Logic&) = delete;

   void SetRenderer(const std::shared_ptr<Renderer>& renderer);

   void ProcessInput(const SDL_KeyboardEvent& key);
   void ProcessInput(const SDL_MouseMotionEvent& motion);
   void ProcessInput(const SDL_MouseButtonEvent& button);
//   void ProcessInput(const kinex::Nui& kinect);

   void Update(int app_time, int elapsed_time);
   void Sound(const std::shared_ptr<Audio>& audio);
   void Render();

   bool Done() const;

private:
   void UpdateUserInterface(int elapsed_time);
   void UpdateMatch(int elapsed_time);

   void SoundUserInterface(Audio& audio);
   void SoundMatch(Audio& audio);

   void RenderUserInterface();
   void RenderMatch();

   bool mDone = false;
   std::shared_ptr<Renderer> mRenderer;
   std::shared_ptr<UserInterface> mUserInterface;
   std::shared_ptr<Match> mMatch;
};

#endif // LOGIC_HPP
