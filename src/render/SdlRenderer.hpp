#ifndef SDL_RENDERER_HPP
#define SDL_RENDERER_HPP

#include "Renderer.hpp"

#include <map>
#include <memory>
#include <sstream>
#include <string>

#include <SDL_ttf.h>

class ResourceCache;
struct Size;
struct SDL_Surface;

class SdlRenderer : public Renderer
{
public:
   SdlRenderer(Size res);
   virtual ~SdlRenderer();

   SdlRenderer(const SdlRenderer&) = delete;
   SdlRenderer& operator=(const SdlRenderer&) = delete;

   void PreRender() override;
   void PostRender() override;

   void Render(const std::shared_ptr<MainMenu>& mainmenu) override;
   void Render(const std::shared_ptr<Arena>& arena) override;
   void Render(const std::shared_ptr<Scoreboard>& scoreboard) override;
   void Render(const std::shared_ptr<Cell>& cell) override;
   void Render(const std::shared_ptr<Wall>& explosion) override;
   void Render(const std::shared_ptr<Extra>& bomb) override;
   void Render(const std::shared_ptr<Bomb>& bomb) override;
   void Render(const std::shared_ptr<Explosion>& explosion) override;
   void Render(const std::shared_ptr<Player>& player) override;

private:
   void Render(const std::shared_ptr<SceneObject>& obj, SDL_Surface* frame);

   // Writing to the video surface is ok since we use double buffering.
   SDL_Surface* mScreen = nullptr;
   TTF_Font* mFont = nullptr;

   // Must be initialized after the video system has been set up.
   std::shared_ptr<ResourceCache> mResCache;
};

#endif // SDL_RENDERER_HPP
