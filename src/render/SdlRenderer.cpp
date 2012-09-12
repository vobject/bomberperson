#include "SdlRenderer.hpp"
#include "../game/UserInterface.hpp"
#include "../game/MainMenu.hpp"
#include "../game/Arena.hpp"
#include "../game/Scoreboard.hpp"
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

   // TODO: Load fonts from ResCache.

   mFont = TTF_OpenFont("res_q1/font/scoreboard.ttf", 16);
   if (!mFont) {
      TTF_Quit();
      throw "TTF_OpenFont() failed!";
   }

   mMenuFont = TTF_OpenFont("res_q1/font/menu.ttf", 72);
   if (!mFont) {
      TTF_Quit();
      throw "TTF_OpenFont() failed!";
   }

   mResCache = make_unique<ResourceCache>();
}

SdlRenderer::~SdlRenderer()
{
   TTF_CloseFont(mMenuFont);
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
   const auto frame = mResCache->GetMenuResource(id).GetFrame();
   Render(mainmenu, frame);

   const auto pos = mainmenu->GetPosition();
   const auto items = mainmenu->GetMenuItems();
   const auto selection = mainmenu->GetSelection();

   for (size_t i = 0; i < items.size(); i++)
   {
      auto surface = TTF_RenderText_Blended(mMenuFont,
                                            items[i].text.c_str(),
                                            { 0xC0, 0xC0, 0xC0, 0 });
      SDL_Rect txt_rect = { static_cast<Sint16>(pos.X + 224),
                            static_cast<Sint16>(pos.Y + 164 + (96 * i)),
                            0,
                            0 };
      SDL_BlitSurface(surface, NULL, mScreen, &txt_rect);
      SDL_FreeSurface(surface);

      if (selection.id == items[i].id)
      {
//         SDL_Rect sel_rect = { static_cast<Sint16>(pos.X + 64),
//                               static_cast<Sint16>(pos.Y + 110 + (96 * i)),
//                               static_cast<Uint16>(48),
//                               static_cast<Uint16>(48) };
//         SDL_FillRect(mScreen, &sel_rect, 0xffff00);

         auto sel_icon = TTF_RenderText_Blended(mMenuFont,
                                                "Q",
                                                { 0x70, 0x70, 0x70, 0 });
         SDL_Rect ico_rect = { static_cast<Sint16>(pos.X + 144),
                               static_cast<Sint16>(pos.Y + 158 + (96 * i)),
                               0,
                               0 };
         SDL_BlitSurface(sel_icon, NULL, mScreen, &ico_rect);
         SDL_FreeSurface(sel_icon);
      }
   }
}

void SdlRenderer::Render(const std::shared_ptr<Arena>& arena)
{
   const auto id = arena->GetId();
   const auto frame = mResCache->GetArenaResource(id).GetFrame();
   Render(arena, frame);
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

   for (size_t i = 0; i < lines.size(); i++)
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
}

void SdlRenderer::Render(const std::shared_ptr<Wall>& wall)
{
   const auto type = wall->GetType();
   const auto frame = mResCache->GetWallResource(type).GetFrame();
   Render(wall, frame);
}

void SdlRenderer::Render(const std::shared_ptr<Extra>& extra)
{
   const auto type = extra->GetType();
   const auto frame = mResCache->GetExtraResource(type).GetFrame();
   Render(extra, frame);
}

void SdlRenderer::Render(const std::shared_ptr<Bomb>& bomb)
{
   const auto id = bomb->GetId();
   const auto anim_time = bomb->GetAnimationTime();
   const auto frame = mResCache->GetBombResource(id).GetFrame(anim_time);
   Render(bomb, frame);
}

void SdlRenderer::Render(const std::shared_ptr<Explosion>& explosion)
{
   const auto id = explosion->GetId();
   const auto anim_time = explosion->GetAnimationTime();
   const auto frame = mResCache->GetExplosionResource(id).GetFrame(anim_time);
   Render(explosion, frame);
}

void SdlRenderer::Render(const std::shared_ptr<Player>& player)
{
   const auto type = player->GetType();
   const auto data = player->GetData();

   const auto res = mResCache->GetPlayerResource(type);
   const auto frame = res.GetFrame(data.anim, data.anim_time, data.speed);
   Render(player, frame);
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
