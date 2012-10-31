#ifndef RAYCAST_RENDERER_HPP
#define RAYCAST_RENDERER_HPP

#include "../Renderer.hpp"
#include "../../utils/Utils.hpp"

#include <vector>

struct SDL_Color;
struct SDL_Surface;

class RaycastRenderer : public Renderer
{
public:
   RaycastRenderer(Size res);
   virtual ~RaycastRenderer();

   RaycastRenderer(const RaycastRenderer&) = delete;
   RaycastRenderer& operator=(const RaycastRenderer&) = delete;

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
   void DrawVerticalLine(int x, int y1, int y2, SDL_Color color);

   const Size mResolution;
   SDL_Surface* mScreen = nullptr;

   std::vector<std::vector<int>> mMap;
   Arena* mArena;
};

#endif // RAYCAST_RENDERER_HPP
