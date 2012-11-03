#include "SimpleSdlRenderer.hpp"
#include "../../game/UserInterface.hpp"
#include "../../game/MenuItem.hpp"
#include "../../game/MainMenu.hpp"
#include "../../game/Arena.hpp"
#include "../../game/Scoreboard.hpp"
#include "../../game/Wall.hpp"
#include "../../game/Extra.hpp"
#include "../../game/Bomb.hpp"
#include "../../game/Explosion.hpp"
#include "../../game/Player.hpp"
#include "../../utils/Utils.hpp"

#include <SDL.h>

SimpleSdlRenderer::SimpleSdlRenderer(const Size res)
{
   if (0 > SDL_Init(SDL_INIT_VIDEO)) {
      throw "Cannot init SDL video subsystem.";
   }
   atexit(SDL_Quit);

   const auto screen = SDL_SetVideoMode(res.Width,
                                        res.Height,
                                        32,
                                        SDL_ANYFORMAT |
                                           SDL_SWSURFACE |
                                           SDL_DOUBLEBUF);
   if (!screen) {
      throw "SDL_SetVideoMode() failed.";
   }

   // The return value of SDL_SetVideoMode() (-> screen) should not be freed
   //  by the caller. The man page tells us to rely on SDL_Quit() to do this.
}

SimpleSdlRenderer::~SimpleSdlRenderer()
{

}

void SimpleSdlRenderer::PreRender()
{
   // Screen size might have changed.
   mScreen = SDL_GetVideoSurface();

   const auto black = SDL_MapRGB(mScreen->format, 0, 0, 0);
   SDL_FillRect(mScreen, NULL, black);
}

void SimpleSdlRenderer::PostRender()
{
   SDL_Flip(mScreen);
}

void SimpleSdlRenderer::Render(const std::shared_ptr<MenuItem>& obj)
{
   const auto pos = obj->GetPosition();
   const auto size = obj->GetSize();

   int color = 0x909090;
   if (obj->IsEnabled()) {
      color = 0xC0C0C0;
   }

   SDL_Rect rect = { static_cast<Sint16>(pos.X),
                     static_cast<Sint16>(pos.Y),
                     static_cast<Uint16>(size.Width),
                     static_cast<Uint16>(size.Height) };
   SDL_FillRect(mScreen, &rect, color);

   if (obj->IsSelected())
   {
      SDL_Rect sel_rect = { static_cast<Sint16>(pos.X - 100),
                            static_cast<Sint16>(pos.Y - 6),
                            static_cast<Uint16>(48),
                            static_cast<Uint16>(48) };
      SDL_FillRect(mScreen, &sel_rect, 0xffff00);
   }
}

void SimpleSdlRenderer::Render(const std::shared_ptr<MainMenu>& obj)
{
   const auto pos = obj->GetPosition();
   const auto size = obj->GetSize();

   SDL_Rect rect = { static_cast<Sint16>(pos.X),
                     static_cast<Sint16>(pos.Y),
                     static_cast<Uint16>(size.Width),
                     static_cast<Uint16>(size.Height) };
   SDL_FillRect(mScreen, &rect, 0x0000af);
}

void SimpleSdlRenderer::Render(const std::shared_ptr<Arena>& arena)
{
   const auto pos = arena->GetPosition();
   const auto size = arena->GetSize();

   SDL_Rect rect = { static_cast<Sint16>(pos.X),
                     static_cast<Sint16>(pos.Y),
                     static_cast<Uint16>(size.Width),
                     static_cast<Uint16>(size.Height) };
   SDL_FillRect(mScreen, &rect, 0x7f7f00);
}

void SimpleSdlRenderer::Render(const std::shared_ptr<Scoreboard>& scoreboard)
{
   // No font resource available to render.
}

void SimpleSdlRenderer::Render(const std::shared_ptr<Wall>& wall)
{
   const auto pos = wall->GetPosition();
   const auto size = wall->GetSize();

   SDL_Rect rect = { static_cast<Sint16>(pos.X),
                     static_cast<Sint16>(pos.Y),
                     static_cast<Uint16>(size.Width),
                     static_cast<Uint16>(size.Height) };

   int color = 0;

   if (WallType::Destructible == wall->GetType())
   {
      color = 0x7f7f7f;
   }
   else
   {
      color = 0x4f4f4f;
   }

   SDL_FillRect(mScreen, &rect, color);
}

void SimpleSdlRenderer::Render(const std::shared_ptr<Extra>& extra)
{
   const auto pos = extra->GetPosition();
   const auto size = extra->GetSize();

   SDL_Rect rect = { static_cast<Sint16>(pos.X),
                     static_cast<Sint16>(pos.Y),
                     static_cast<Uint16>(size.Width),
                     static_cast<Uint16>(size.Height) };

   int color = 0;

   switch (extra->GetType())
   {
      case ExtraType::Speed:
         color = 0xff0000;
         break;
      case ExtraType::Range:
         color = 0x00ff00;
         break;
      case ExtraType::Bombs:
         color = 0x0000ff;
         break;
      case ExtraType::InfiniteRange:
         color = 0xff1fff;
         break;
      default:
         break;
   }

   SDL_FillRect(mScreen, &rect, color);
}

void SimpleSdlRenderer::Render(const std::shared_ptr<Bomb>& bomb)
{
   const auto pos = bomb->GetPosition();
   const auto size = bomb->GetSize();

   SDL_Rect rect = { static_cast<Sint16>(pos.X),
                     static_cast<Sint16>(pos.Y),
                     static_cast<Uint16>(size.Width),
                     static_cast<Uint16>(size.Height) };
   SDL_FillRect(mScreen, &rect, 0x000000);
}

void SimpleSdlRenderer::Render(const std::shared_ptr<Explosion>& explosion)
{
   const auto pos = explosion->GetPosition();
   const auto size = explosion->GetSize();

   SDL_Rect rect = { static_cast<Sint16>(pos.X),
                     static_cast<Sint16>(pos.Y),
                     static_cast<Uint16>(size.Width),
                     static_cast<Uint16>(size.Height) };
   SDL_FillRect(mScreen, &rect, 0xffff00);
}

void SimpleSdlRenderer::Render(const std::shared_ptr<Player>& player)
{
   const auto pos = player->GetPosition();
   const auto size = player->GetSize();

   SDL_Rect rect = { static_cast<Sint16>(pos.X),
                     static_cast<Sint16>(pos.Y),
                     static_cast<Uint16>(size.Width),
                     static_cast<Uint16>(size.Height) };
   SDL_FillRect(mScreen, &rect, 0x00afaf);
}
