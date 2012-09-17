#ifndef CURSES_RENDERER_HPP
#define CURSES_RENDERER_HPP

#include "Renderer.hpp"

struct Size;

class CursesRenderer : public Renderer
{
public:
   CursesRenderer(Size res);
   virtual ~CursesRenderer();

   CursesRenderer(const CursesRenderer&) = delete;
   CursesRenderer& operator=(const CursesRenderer&) = delete;

   void PreRender() override;
   void PostRender() override;

   void Render(const std::shared_ptr<MainMenu>& mainmenu) override;
   void Render(const std::shared_ptr<Arena>& arena) override;
   void Render(const std::shared_ptr<Scoreboard>& scoreboard) override;
   void Render(const std::shared_ptr<Wall>& explosion) override;
   void Render(const std::shared_ptr<Extra>& bomb) override;
   void Render(const std::shared_ptr<Bomb>& bomb) override;
   void Render(const std::shared_ptr<Explosion>& explosion) override;
   void Render(const std::shared_ptr<Player>& player) override;

private:
   float mXResFactor = .0f;
   float mYResFactor = .0f;
};

#endif // CURSES_RENDERER_HPP
