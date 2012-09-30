#include "SimpleSdlRenderer.hpp"
#include "../game/UserInterface.hpp"
#include "../game/MainMenu.hpp"
#include "../game/Arena.hpp"
#include "../game/Scoreboard.hpp"
#include "../game/Wall.hpp"
#include "../game/Extra.hpp"
#include "../game/Bomb.hpp"
#include "../game/Explosion.hpp"
#include "../game/Player.hpp"
#include "../utils/Utils.hpp"

#include <SDL.h>

//SimpleSdlRenderer::SimpleSdlRenderer(const Size res)
//{
//   if (0 > SDL_Init(SDL_INIT_VIDEO)) {
//      throw "Cannot init SDL video subsystem.";
//   }
//   atexit(SDL_Quit);

//   const auto screen = SDL_SetVideoMode(res.Width,
//                                        res.Height,
//                                        32,
//                                        SDL_ANYFORMAT |
//                                           SDL_SWSURFACE |
//                                           SDL_DOUBLEBUF);
//   if (!screen) {
//      throw "SDL_SetVideoMode() failed.";
//   }

//   // The return value of SDL_SetVideoMode() (-> screen) should not be freed
//   //  by the caller. The man page tells us to rely on SDL_Quit() to do this.
//}

//SimpleSdlRenderer::~SimpleSdlRenderer()
//{

//}

//void SimpleSdlRenderer::PreRender()
//{
//   // Screen size might have changed.
//   mScreen = SDL_GetVideoSurface();

//   const auto black = SDL_MapRGB(mScreen->format, 0, 0, 0);
//   SDL_FillRect(mScreen, NULL, black);
//}

//void SimpleSdlRenderer::PostRender()
//{
//   SDL_Flip(mScreen);
//}

//void SimpleSdlRenderer::Render(const std::shared_ptr<MenuItem>& obj)
//{

//}

//void SimpleSdlRenderer::Render(const std::shared_ptr<MenuItemSelector>& obj)
//{

//}

//void SimpleSdlRenderer::Render(const std::shared_ptr<MainMenu>& mainmenu)
//{
////   const auto pos = mainmenu->GetPosition();
////   const auto size = mainmenu->GetSize();

////   SDL_Rect rect = { static_cast<Sint16>(pos.X),
////                     static_cast<Sint16>(pos.Y),
////                     static_cast<Uint16>(size.Width),
////                     static_cast<Uint16>(size.Height) };
////   SDL_FillRect(mScreen, &rect, 0x7f7f6f);

////   const auto items = mainmenu->GetMenuItems();
////   const auto selection = mainmenu->GetSelection();

////   for (size_t i = 0; i < items.size(); i++)
////   {
////      SDL_Rect item_rect = { static_cast<Sint16>(pos.X + 128),
////                             static_cast<Sint16>(pos.Y + 96 + (96 * i)),
////                             static_cast<Uint16>(384),
////                             static_cast<Uint16>(64) };
////      SDL_FillRect(mScreen, &item_rect, 0xffffff);

////      if (selection.id == items[i].id)
////      {
////         SDL_Rect sel_rect = { static_cast<Sint16>(pos.X + 64),
////                               static_cast<Sint16>(pos.Y + 110 + (96 * i)),
////                               static_cast<Uint16>(48),
////                               static_cast<Uint16>(48) };
////         SDL_FillRect(mScreen, &sel_rect, 0xffff00);
////      }
////   }
//}

//void SimpleSdlRenderer::Render(const std::shared_ptr<Arena>& arena)
//{
//   const auto pos = arena->GetPosition();
//   const auto size = arena->GetSize();

//   SDL_Rect rect = { static_cast<Sint16>(pos.X),
//                     static_cast<Sint16>(pos.Y),
//                     static_cast<Uint16>(size.Width),
//                     static_cast<Uint16>(size.Height) };
//   SDL_FillRect(mScreen, &rect, 0x7f7f00);
//}

//void SimpleSdlRenderer::Render(const std::shared_ptr<Scoreboard>& scoreboard)
//{
//   // No font resource available to render.
//}

//void SimpleSdlRenderer::Render(const std::shared_ptr<Wall>& wall)
//{
//   const auto pos = wall->GetPosition();
//   const auto size = wall->GetSize();

//   SDL_Rect rect = { static_cast<Sint16>(pos.X),
//                     static_cast<Sint16>(pos.Y),
//                     static_cast<Uint16>(size.Width),
//                     static_cast<Uint16>(size.Height) };

//   int color = 0;

//   if (WallType::Destructible == wall->GetType())
//   {
//      color = 0x7f7f7f;
//   }
//   else
//   {
//      color = 0x4f4f4f;
//   }

//   SDL_FillRect(mScreen, &rect, color);
//}

//void SimpleSdlRenderer::Render(const std::shared_ptr<Extra>& extra)
//{
//   const auto pos = extra->GetPosition();
//   const auto size = extra->GetSize();

//   SDL_Rect rect = { static_cast<Sint16>(pos.X),
//                     static_cast<Sint16>(pos.Y),
//                     static_cast<Uint16>(size.Width),
//                     static_cast<Uint16>(size.Height) };

//   int color = 0;

//   switch (extra->GetType())
//   {
//      case ExtraType::Speed:
//         color = 0xff0000;
//         break;
//      case ExtraType::Range:
//         color = 0x00ff00;
//         break;
//      case ExtraType::Bombs:
//         color = 0x0000ff;
//         break;
//      case ExtraType::InfiniteRange:
//         color = 0xff1fff;
//         break;
//      default:
//         break;
//   }

//   SDL_FillRect(mScreen, &rect, color);
//}

//void SimpleSdlRenderer::Render(const std::shared_ptr<Bomb>& bomb)
//{
//   const auto pos = bomb->GetPosition();
//   const auto size = bomb->GetSize();

//   SDL_Rect rect = { static_cast<Sint16>(pos.X),
//                     static_cast<Sint16>(pos.Y),
//                     static_cast<Uint16>(size.Width),
//                     static_cast<Uint16>(size.Height) };
//   SDL_FillRect(mScreen, &rect, 0x000000);
//}

//void SimpleSdlRenderer::Render(const std::shared_ptr<Explosion>& explosion)
//{
//   const auto pos = explosion->GetPosition();
//   const auto size = explosion->GetSize();

//   SDL_Rect rect = { static_cast<Sint16>(pos.X),
//                     static_cast<Sint16>(pos.Y),
//                     static_cast<Uint16>(size.Width),
//                     static_cast<Uint16>(size.Height) };
//   SDL_FillRect(mScreen, &rect, 0xffff00);
//}

//void SimpleSdlRenderer::Render(const std::shared_ptr<Player>& player)
//{
//   const auto pos = player->GetPosition();
//   const auto size = player->GetSize();

//   SDL_Rect rect = { static_cast<Sint16>(pos.X),
//                     static_cast<Sint16>(pos.Y),
//                     static_cast<Uint16>(size.Width),
//                     static_cast<Uint16>(size.Height) };
//   SDL_FillRect(mScreen, &rect, 0x00afaf);
//}

////void SimpleSdlRenderer::DrawLine(const Point& src_pos, const Point& dest_pos, const unsigned int color)
////{
////   // based on http://alawibaba.com/projects/whiteboard/drawing-SDL.c
////
////#define SGN(x) ((x)>0 ? 1 : ((x)==0 ? 0:(-1)))
////#define ABS(x) ((x)>0 ? (x) : (-x))
////
////   int x1 = src_pos.X;
////   int y1 = src_pos.Y;
////   int x2 = dest_pos.X;
////   int y2 = dest_pos.Y;
////
////   int lg_delta;
////   int sh_delta;
////   int cycle;
////   int lg_step;
////   int sh_step;
////
////   lg_delta = x2 - x1;
////   sh_delta = y2 - y1;
////   lg_step = SGN(lg_delta);
////   lg_delta = ABS(lg_delta);
////   sh_step = SGN(sh_delta);
////   sh_delta = ABS(sh_delta);
////
////   if (sh_delta < lg_delta)
////   {
////      cycle = lg_delta >> 1;
////      while (x1 != x2)
////      {
////         DrawPixel({x1, y1}, color);
////
////         cycle += sh_delta;
////         if (cycle > lg_delta)
////         {
////            cycle -= lg_delta;
////            y1 += sh_step;
////         }
////         x1 += lg_step;
////      }
////      DrawPixel({x1, y1}, color);
////   }
////
////   cycle = sh_delta >> 1;
////   while (y1 != y2)
////   {
////      DrawPixel({x1, y1}, color);
////
////      cycle += lg_delta;
////      if (cycle > sh_delta)
////      {
////         cycle -= sh_delta;
////         x1 += lg_step;
////      }
////      y1 += sh_step;
////   }
////   DrawPixel({x1, y1}, color);
////}
////
////void SimpleSdlRenderer::DrawPixel(const Point& pos, const unsigned int color)
////{
////   const auto bpp = mScreen->format->BytesPerPixel;
////   const auto offset = (mScreen->pitch * pos.Y) + (pos.X * bpp);
////
////   SDL_LockSurface(mScreen);
////   memcpy(static_cast<char*>(mScreen->pixels) + offset, &color, bpp);
////   SDL_UnlockSurface(mScreen);
////}
