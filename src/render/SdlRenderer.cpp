#include "SdlRenderer.hpp"
#include "../game/MainMenu.hpp"
#include "../game/Match.hpp"
#include "../game/Arena.hpp"
#include "../game/Cell.hpp"
#include "../game/Wall.hpp"
#include "../game/Extra.hpp"
#include "../game/Bomb.hpp"
#include "../game/Explosion.hpp"
#include "../game/Player.hpp"
#include "../resource/ResourceCache.hpp"
#include "../utils/Utils.hpp"

#include <SDL.h>

SdlRenderer::SdlRenderer(const Size res)
{
   if (0 > SDL_Init(SDL_INIT_VIDEO)) {
      throw "Cannot init SDL video subsystem.";
   }
   atexit(SDL_Quit);

   mScreen = SDL_SetVideoMode(res.Width,
                              res.Height,
                              32,
                              SDL_ANYFORMAT |
                                 SDL_SWSURFACE |
                                 SDL_DOUBLEBUF);
   if (!mScreen) {
      throw "SDL_SetVideoMode() failed.";
   }

   // The return value of SDL_SetVideoMode() (-> screen) should not be freed
   //  by the caller. The man page tells us to rely on SDL_Quit() to do this.

   mResCache = std::make_shared<ResourceCache>();
}

SdlRenderer::~SdlRenderer()
{

}

void SdlRenderer::PreRender()
{

}

void SdlRenderer::PostRender()
{
   SDL_Flip(mScreen);
}

void SdlRenderer::Render(const std::shared_ptr<MainMenu>& mainmenu)
{
   const auto pos = mainmenu->GetPosition();
   const auto size = mainmenu->GetSize();
   const auto selection = mainmenu->GetSelection();

   const int border_x = .1f * size.Width;
   const int border_y = .1f * size.Height;

   SDL_Rect rect = { static_cast<Sint16>(pos.X + border_x),
                     static_cast<Sint16>(pos.Y + border_y),
                     static_cast<Uint16>(size.Width - (2 * border_x)),
                     static_cast<Uint16>(size.Height - (2 * border_y)) };
   SDL_FillRect(mScreen, &rect, 0x9f9f7f);

   const auto menu_item_cnt = static_cast<int>(MainMenuItem::Exit) + 1;
   const auto selected_item = static_cast<int>(selection);
   for (int i = 0; i < menu_item_cnt; i++)
   {
      int item_color = 0x000000;
      if (selected_item == i) {
         item_color = 0xffffff;
      }

      SDL_Rect item_rect = { static_cast<Sint16>(pos.X + (2 * border_x)),
                             static_cast<Sint16>(pos.Y + (2 * border_y) + (border_y * i)),
                             static_cast<Uint16>(10),
                             static_cast<Uint16>(10) };
      SDL_FillRect(mScreen, &item_rect, item_color);
   }
}

void SdlRenderer::Render(const std::shared_ptr<Match>& match)
{
   // FIXME: Match is not a SceneObject! Is that a design problem?
   // Who should orchestrate the rendering of the various objects?
   // Currently the renderer is responsible for it.

   // TODO: Get Match statistics and render them to the screen.

   Render(match->GetArena());

   for (const auto& player : match->GetPlayers())
   {
      Render(player);
   }
}

void SdlRenderer::Render(const std::shared_ptr<Arena>& arena)
{
   const auto name = arena->GetId();
   const auto frame = mResCache->GetArenaResource(name).GetFrame(0);
   Render(arena, frame);

   for (const auto& cell : arena->GetCells())
   {
      Render(cell);
   }
}

void SdlRenderer::Render(const std::shared_ptr<Cell>& cell)
{
   if (cell->HasWall())
   {
      Render(cell->GetWall());
      return;
   }

   if (cell->HasExtra())
   {
      Render(cell->GetExtra());
   }

   if (cell->HasBomb())
   {
      Render(cell->GetBomb());
   }

   if (cell->HasExplosion())
   {
      Render(cell->GetExplosion());
   }
}

void SdlRenderer::Render(const std::shared_ptr<Wall>& wall)
{
   const auto name = wall->GetId();
   const auto frame = mResCache->GetWallResource(name).GetFrame(0);
   Render(wall, frame);
}

void SdlRenderer::Render(const std::shared_ptr<Extra>& extra)
{
   const auto name = extra->GetId();
   const auto frame = mResCache->GetExtraResource(name).GetFrame(0);
   Render(extra, frame);
}

void SdlRenderer::Render(const std::shared_ptr<Bomb>& bomb)
{
   const auto name = bomb->GetId();
   const auto index = bomb->GetAnimationFrame();
   const auto frame = mResCache->GetBombResource(name).GetFrame(index);
   Render(bomb, frame);
}

void SdlRenderer::Render(const std::shared_ptr<Explosion>& explosion)
{
   const auto name = explosion->GetId();
   const auto index = explosion->GetAnimationFrame();
   const auto frame = mResCache->GetExplosionResource(name).GetFrame(index);
   Render(explosion, frame);
}

void SdlRenderer::Render(const std::shared_ptr<Player>& player)
{
   const auto name = player->GetId();
   const auto dir = player->GetDirection();
   const auto index = player->GetAnimationFrame();

   const auto res = mResCache->GetPlayerResource(name);
   const auto frame = res.GetWalkFrame(dir, index);
   Render(player, frame);
}

void SdlRenderer::Render(const std::shared_ptr<SceneObject>& obj)
{
   LOG(logDEBUG) << "SdlRenderer::Render(SceneObject) not implemented!";
}

void SdlRenderer::Render(
   const std::shared_ptr<SceneObject>& obj,
   SDL_Surface* frame
)
{
   const auto pos = obj->GetPosition();
   const auto size = obj->GetSize();

   SDL_Rect rect = { static_cast<Sint16>(pos.X),
                     static_cast<Sint16>(pos.Y),
                     static_cast<Uint16>(size.Width),
                     static_cast<Uint16>(size.Height) };
   SDL_BlitSurface(frame, NULL, mScreen, &rect);
}
