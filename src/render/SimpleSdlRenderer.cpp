#include "SimpleSdlRenderer.hpp"
#include "../game/MainMenu.hpp"
#include "../game/Match.hpp"
#include "../game/Arena.hpp"
#include "../game/Cell.hpp"
#include "../game/Wall.hpp"
#include "../game/Extra.hpp"
#include "../game/Bomb.hpp"
#include "../game/Explosion.hpp"
#include "../game/Player.hpp"
#include "../utils/Utils.hpp"

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

void SimpleSdlRenderer::Render(const std::shared_ptr<MainMenu>& mainmenu)
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
   SDL_FillRect(mScreen, &rect, 0x7f7f6f);

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

void SimpleSdlRenderer::Render(const std::shared_ptr<Match>& match)
{
   Render(match->GetArena());

   for (const auto& player : match->GetPlayers())
   {
      Render(player);
   }
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

   for (const auto& cell : arena->GetCells())
   {
      Render(cell);
   }
}

void SimpleSdlRenderer::Render(const std::shared_ptr<Cell>& cell)
{
   if (cell->HasWall()) {
      Render(cell->GetWall());
      return;
   }

   if (cell->HasExtra()) {
      Render(cell->GetExtra());
   }

   if (cell->HasBomb()) {
      Render(cell->GetBomb());
   }

   if (cell->HasExplosion()) {
      Render(cell->GetExplosion());
   }
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

   if (wall->IsDestructible())
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

   switch (extra->GetId())
   {
      case EntityId::SpeedExtra:
         color = 0xff0000;
         break;
      case EntityId::RangeExtra:
         color = 0x00ff00;
         break;
      case EntityId::BombsExtra:
         color = 0x0000ff;
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

void SimpleSdlRenderer::Render(const std::shared_ptr<SceneObject>& obj)
{
   const auto pos = obj->GetPosition();
   const auto size = obj->GetSize();

   SDL_Rect rect = { static_cast<Sint16>(pos.X),
                     static_cast<Sint16>(pos.Y),
                     static_cast<Uint16>(size.Width),
                     static_cast<Uint16>(size.Height) };
   SDL_FillRect(mScreen, &rect, 0xffffff);
}

//void SimpleSdlRenderer::DrawLine(const Point& src_pos, const Point& dest_pos, const unsigned int color)
//{
//   // based on http://alawibaba.com/projects/whiteboard/drawing-SDL.c
//
//#define SGN(x) ((x)>0 ? 1 : ((x)==0 ? 0:(-1)))
//#define ABS(x) ((x)>0 ? (x) : (-x))
//
//   int x1 = src_pos.X;
//   int y1 = src_pos.Y;
//   int x2 = dest_pos.X;
//   int y2 = dest_pos.Y;
//
//   int lg_delta;
//   int sh_delta;
//   int cycle;
//   int lg_step;
//   int sh_step;
//
//   lg_delta = x2 - x1;
//   sh_delta = y2 - y1;
//   lg_step = SGN(lg_delta);
//   lg_delta = ABS(lg_delta);
//   sh_step = SGN(sh_delta);
//   sh_delta = ABS(sh_delta);
//
//   if (sh_delta < lg_delta)
//   {
//      cycle = lg_delta >> 1;
//      while (x1 != x2)
//      {
//         DrawPixel({x1, y1}, color);
//
//         cycle += sh_delta;
//         if (cycle > lg_delta)
//         {
//            cycle -= lg_delta;
//            y1 += sh_step;
//         }
//         x1 += lg_step;
//      }
//      DrawPixel({x1, y1}, color);
//   }
//
//   cycle = sh_delta >> 1;
//   while (y1 != y2)
//   {
//      DrawPixel({x1, y1}, color);
//
//      cycle += lg_delta;
//      if (cycle > sh_delta)
//      {
//         cycle -= sh_delta;
//         x1 += lg_step;
//      }
//      y1 += sh_step;
//   }
//   DrawPixel({x1, y1}, color);
//}
//
//void SimpleSdlRenderer::DrawPixel(const Point& pos, const unsigned int color)
//{
//   const auto bpp = mScreen->format->BytesPerPixel;
//   const auto offset = (mScreen->pitch * pos.Y) + (pos.X * bpp);
//
//   SDL_LockSurface(mScreen);
//   memcpy(static_cast<char*>(mScreen->pixels) + offset, &color, bpp);
//   SDL_UnlockSurface(mScreen);
//}
