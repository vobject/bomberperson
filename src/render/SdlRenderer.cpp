#include "SdlRenderer.hpp"
#include "../game/MainMenu.hpp"
#include "../game/Match.hpp"
#include "../game/Arena.hpp"
#include "../game/Scoreboard.hpp"
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

   // The return value of SDL_SetVideoMode() (-> mScreen) should not be freed
   //  by the caller. The man page tells us to rely on SDL_Quit() to do this.

   if (0 > TTF_Init()) {
      throw "Cannot init SDL ttf feature.";
   }

   mFont = TTF_OpenFont("VeraMono.ttf", 16);
   if (!mFont) {
      TTF_Quit();
      throw "TTF_OpenFont() failed!";
   }

   mResCache = std::make_shared<ResourceCache>();
}

SdlRenderer::~SdlRenderer()
{
   TTF_CloseFont(mFont);
   TTF_Quit();
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
   const auto id = mainmenu->GetId();
   const auto frame = mResCache->GetMenuResource(id).GetFrame(0);
   Render(mainmenu, frame);

   const auto pos = mainmenu->GetPosition();
   const auto selection = mainmenu->GetSelection();

   const auto menu_item_cnt = static_cast<int>(MainMenuItem::Exit) + 1;
   const auto selected_item = static_cast<int>(selection);
   for (int i = 0; i < menu_item_cnt; i++)
   {
      int item_color = 0xc0c0c0;
      if (selected_item == i) {
         item_color = 0x804000;
      }

      SDL_Rect item_rect = { static_cast<Sint16>(pos.X + 70),
                             static_cast<Sint16>(pos.Y + 55 + (55 * i)),
                             static_cast<Uint16>(20),
                             static_cast<Uint16>(20) };
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
   Render(match->GetScoreboard());

   for (const auto& player : match->GetPlayers())
   {
      Render(player);
   }
}

void SdlRenderer::Render(const std::shared_ptr<Arena>& arena)
{
   const auto id = arena->GetId();
   const auto frame = mResCache->GetArenaResource(id).GetFrame(0);
   Render(arena, frame);

   for (const auto& cell : arena->GetCells())
   {
      Render(cell);
   }
}

void SdlRenderer::Render(const std::shared_ptr<Scoreboard>& scoreboard)
{
   const auto pos = scoreboard->GetPosition();
   const auto size = scoreboard->GetSize();

   SDL_Rect rect = { static_cast<Sint16>(pos.X),
                     static_cast<Sint16>(pos.Y),
                     static_cast<Uint16>(size.Width),
                     static_cast<Uint16>(size.Height) };
   SDL_FillRect(mScreen, &rect, 0x1f1f00);

   const auto line_dist = 20;
   const auto lines = scoreboard->GetScore();

   for (int i = 0; i < lines.size(); i++)
   {
      auto surface = TTF_RenderText_Blended(mFont, lines[i].c_str(), { 0xff,
                                                                       0xff,
                                                                       0xff,
                                                                       0 });
      SDL_Rect txt_rect = { static_cast<Sint16>(pos.X + 5),
                            static_cast<Sint16>(pos.Y + 5 + (line_dist * i)),
                            0,
                            0 };

      SDL_BlitSurface(surface, NULL, mScreen, &txt_rect);
      SDL_FreeSurface(surface);
   }

//   for (const auto& line : scoreboard->GetScore()) {
//      LOG(logDEBUG) << line;
//   }
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
   const auto id = wall->GetId();
   const auto frame = mResCache->GetWallResource(id).GetFrame(0);
   Render(wall, frame);
}

void SdlRenderer::Render(const std::shared_ptr<Extra>& extra)
{
   const auto id = extra->GetId();
   const auto frame = mResCache->GetExtraResource(id).GetFrame(0);
   Render(extra, frame);
}

void SdlRenderer::Render(const std::shared_ptr<Bomb>& bomb)
{
   const auto id = bomb->GetId();
   const auto index = bomb->GetAnimationFrame();
   const auto frame = mResCache->GetBombResource(id).GetFrame(index);
   Render(bomb, frame);
}

void SdlRenderer::Render(const std::shared_ptr<Explosion>& explosion)
{
   const auto id = explosion->GetId();
   const auto index = explosion->GetAnimationFrame();
   const auto frame = mResCache->GetExplosionResource(id).GetFrame(index);
   Render(explosion, frame);
}

void SdlRenderer::Render(const std::shared_ptr<Player>& player)
{
   const auto id = player->GetId();
   const auto dir = player->GetDirection();
   const auto index = player->GetAnimationFrame();

   const auto res = mResCache->GetPlayerResource(id);
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
