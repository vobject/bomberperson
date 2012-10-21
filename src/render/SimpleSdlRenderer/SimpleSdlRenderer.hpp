#ifndef SIMPLE_SDL_RENDERER_HPP
#define SIMPLE_SDL_RENDERER_HPP

#include "../Renderer.hpp"

struct Size;
struct SDL_Surface;

class SimpleSdlRenderer : public Renderer
{
public:
   SimpleSdlRenderer(Size res);
   virtual ~SimpleSdlRenderer();

   SimpleSdlRenderer(const SimpleSdlRenderer&) = delete;
   SimpleSdlRenderer& operator=(const SimpleSdlRenderer&) = delete;

   void PreRender() override;
   void PostRender() override;

   void Render(const std::shared_ptr<MenuItem>& obj) override;
   void Render(const std::shared_ptr<MenuItemSelector>& obj) override;
   void Render(const std::shared_ptr<MainMenu>& obj) override;

   void Render(const std::shared_ptr<Arena>& arena) override;
   void Render(const std::shared_ptr<Scoreboard>& scoreboard) override;
   void Render(const std::shared_ptr<Wall>& explosion) override;
   void Render(const std::shared_ptr<Extra>& bomb) override;
   void Render(const std::shared_ptr<Bomb>& bomb) override;
   void Render(const std::shared_ptr<Explosion>& explosion) override;
   void Render(const std::shared_ptr<Player>& player) override;

private:
   // Writing to the video surface is ok since we use double buffering.
   SDL_Surface* mScreen = nullptr;
};

#endif // SIMPLE_SDL_RENDERER_HPP
