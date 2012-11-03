#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

class SceneObject;

class MenuItem;
class MainMenu;

class Arena;
class Scoreboard;
class Wall;
class Extra;
class Bomb;
class Explosion;
class Player;

class Renderer
{
public:
   Renderer();
   virtual ~Renderer();

   virtual void PreRender();
   virtual void PostRender();
   virtual void Render(const std::shared_ptr<SceneObject>& obj);

   virtual void Render(const std::shared_ptr<MenuItem>& obj) = 0;
   virtual void Render(const std::shared_ptr<MainMenu>& obj) = 0;

   virtual void Render(const std::shared_ptr<Arena>& arena) = 0;
   virtual void Render(const std::shared_ptr<Scoreboard>& scoreboard) = 0;
   virtual void Render(const std::shared_ptr<Wall>& wall) = 0;
   virtual void Render(const std::shared_ptr<Extra>& extra) = 0;
   virtual void Render(const std::shared_ptr<Bomb>& bomb) = 0;
   virtual void Render(const std::shared_ptr<Explosion>& explosion) = 0;
   virtual void Render(const std::shared_ptr<Player>& player) = 0;
};

#endif // RENDERER_HPP
