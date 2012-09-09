#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

class SceneObject;
class MainMenu;
class Arena;
class Scoreboard;
class Cell;
class Wall;
class Extra;
class Bomb;
class Explosion;
class Player;

class Renderer
{
public:
   Renderer() { }
   virtual ~Renderer() { }

   virtual void PreRender() { }
   virtual void PostRender() { }

   virtual void Render(const std::shared_ptr<SceneObject>& obj) = 0;
   virtual void Render(const std::shared_ptr<MainMenu>& mainmenu) = 0;
   virtual void Render(const std::shared_ptr<Arena>& arena) = 0;
   virtual void Render(const std::shared_ptr<Scoreboard>& scoreboard) = 0;
   virtual void Render(const std::shared_ptr<Cell>& cell) = 0;
   virtual void Render(const std::shared_ptr<Wall>& wall) = 0;
   virtual void Render(const std::shared_ptr<Extra>& extra) = 0;
   virtual void Render(const std::shared_ptr<Bomb>& bomb) = 0;
   virtual void Render(const std::shared_ptr<Explosion>& explosion) = 0;
   virtual void Render(const std::shared_ptr<Player>& player) = 0;
};

#endif // RENDERER_HPP
