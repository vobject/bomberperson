#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <memory>

class MainMenu;
class Match;
class Arena;
class Cell;
class Wall;
class Extra;
class Bomb;
class Explosion;
class Player;
class SceneObject;

class Renderer
{
public:
   Renderer() { }
   virtual ~Renderer() { }

   virtual void PreRender() { }
   virtual void PostRender() { }
   virtual void Render(const std::shared_ptr<MainMenu>& mainmenu) = 0;
   virtual void Render(const std::shared_ptr<Match>& match) = 0;
   virtual void Render(const std::shared_ptr<Arena>& field) = 0;
   virtual void Render(const std::shared_ptr<Cell>& cell) = 0;
   virtual void Render(const std::shared_ptr<Wall>& wall) = 0;
   virtual void Render(const std::shared_ptr<Extra>& extra) = 0;
   virtual void Render(const std::shared_ptr<Bomb>& bomb) = 0;
   virtual void Render(const std::shared_ptr<Explosion>& explosion) = 0;
   virtual void Render(const std::shared_ptr<Player>& player) = 0;
   virtual void Render(const std::shared_ptr<SceneObject>& obj) = 0;
};

#endif // RENDERER_HPP
