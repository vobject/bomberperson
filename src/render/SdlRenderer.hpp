#ifndef SDL_RENDERER_HPP
#define SDL_RENDERER_HPP

#include "Renderer.hpp"

#include <map>
#include <memory>
#include <sstream>
#include <string>

class ResourceCache;
struct Size;
struct SDL_Surface;

class SdlRenderer : public Renderer
{
public:
   explicit SdlRenderer(Size res);
   virtual ~SdlRenderer();

   SdlRenderer(const SdlRenderer&) = delete;
   SdlRenderer& operator=(const SdlRenderer&) = delete;

   void PreRender() override;
   void PostRender() override;

   void Render(const std::shared_ptr<MainMenu>& mainmenu) override;
   void Render(const std::shared_ptr<Match>& match) override;
   void Render(const std::shared_ptr<Arena>& field) override;
   void Render(const std::shared_ptr<Cell>& cell) override;
   void Render(const std::shared_ptr<Wall>& explosion) override;
   void Render(const std::shared_ptr<Extra>& bomb) override;
   void Render(const std::shared_ptr<Bomb>& bomb) override;
   void Render(const std::shared_ptr<Explosion>& explosion) override;
   void Render(const std::shared_ptr<Player>& player) override;
   void Render(const std::shared_ptr<SceneObject>& obj) override;

private:
   void Render(const std::shared_ptr<SceneObject>& obj, SDL_Surface* frame);

   // Writing to the video surface is ok since we use double buffering.
   SDL_Surface* mScreen = nullptr;

   // Must be initialized after the video system has been set up.
   std::shared_ptr<ResourceCache> mResCache = nullptr;
};

#endif // SDL_RENDERER_HPP
