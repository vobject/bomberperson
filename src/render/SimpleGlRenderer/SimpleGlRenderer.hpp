#ifndef SIMPLE_GL_RENDERER_HPP
#define SIMPLE_GL_RENDERER_HPP

#include "../Renderer.hpp"

struct Size;
struct SDL_Surface;

class SimpleGlRenderer : public Renderer
{
public:
   SimpleGlRenderer(Size res);
   virtual ~SimpleGlRenderer();

   SimpleGlRenderer(const SimpleGlRenderer&) = delete;
   SimpleGlRenderer& operator=(const SimpleGlRenderer&) = delete;

   void PreRender() override;
   void PostRender() override;

   void Render(const std::shared_ptr<MenuItem>& obj) override;
   void Render(const std::shared_ptr<MainMenu>& obj) override;

   void Render(const std::shared_ptr<Arena>& arena) override;
   void Render(const std::shared_ptr<Scoreboard>& scoreboard) override;
   void Render(const std::shared_ptr<Wall>& explosion) override;
   void Render(const std::shared_ptr<Extra>& bomb) override;
   void Render(const std::shared_ptr<Bomb>& bomb) override;
   void Render(const std::shared_ptr<Explosion>& explosion) override;
   void Render(const std::shared_ptr<Player>& player) override;

private:
   SDL_Surface* mScreen = nullptr;
};

#endif // SIMPLE_GL_RENDERER_HPP
