#ifndef LOGIC_HPP
#define LOGIC_HPP

#include <memory>
#include <list>

struct SDL_KeyboardEvent;

class Renderer;
class Background;
class ArenaGenerator;
class Match;

class Logic
{
public:
   Logic(const std::shared_ptr<Renderer>& renderer);
   virtual ~Logic();

   Logic(const Logic&) = delete;
   Logic& operator=(const Logic&) = delete;

   virtual void ProcessInput(const SDL_KeyboardEvent& ev);
//   virtual void ProcessInput(const kinex::Nui& kinect);

   virtual void Update(int app_time, int elapsed_time);
   virtual void Render();

private:
   std::shared_ptr<Renderer> mRenderer;
   std::shared_ptr<Background> mBackground;
   std::shared_ptr<ArenaGenerator> mFieldGen;
   std::shared_ptr<Match> mMatch;
};

#endif // LOGIC_HPP
