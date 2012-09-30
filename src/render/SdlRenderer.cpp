#include "SdlRenderer.hpp"
#include "SdlRenderer/ResourceCache.hpp"
#include "../game/UserInterface.hpp"
#include "../game/MenuItem.hpp"
#include "../game/MenuItemSelector.hpp"
#include "../game/MainMenu.hpp"
#include "../game/Arena.hpp"
#include "../game/Scoreboard.hpp"
#include "../game/Wall.hpp"
#include "../game/Extra.hpp"
#include "../game/Bomb.hpp"
#include "../game/Explosion.hpp"
#include "../game/Player.hpp"
#include "../utils/Utils.hpp"
#include "../Options.hpp"

#include <SDL.h>

//SdlRenderer::SdlRenderer(const Size res)
//{
//   if (0 > SDL_Init(SDL_INIT_VIDEO)) {
//      throw "Cannot init SDL video subsystem.";
//   }
//   atexit(SDL_Quit);

//   mScreen = SDL_SetVideoMode(res.Width,
//                              res.Height,
//                              32,
//                              SDL_ANYFORMAT |
//                                 SDL_SWSURFACE |
//                                 SDL_DOUBLEBUF);
//   if (!mScreen) {
//      throw "SDL_SetVideoMode() failed.";
//   }

//   // The return value of SDL_SetVideoMode() (-> mScreen) should not be freed
//   //  by the caller. The man page tells us to rely on SDL_Quit() to do this.

//   if (0 > TTF_Init()) {
//      throw "Cannot init SDL ttf feature.";
//   }

//   // TODO: Load fonts from ResCache.

//   mFont = TTF_OpenFont("res_q1/render/SdlRenderer/font/scoreboard.ttf", 16);
//   if (!mFont) {
//      TTF_Quit();
//      throw "TTF_OpenFont() failed!";
//   }

//   mMenuFont = TTF_OpenFont("res_q1/render/SdlRenderer/font/menu.ttf", 72);
//   if (!mFont) {
//      TTF_Quit();
//      throw "TTF_OpenFont() failed!";
//   }

//   mResCache = make_unique<ResourceCache>("SdlRenderer");
//}

//SdlRenderer::~SdlRenderer()
//{
//   TTF_CloseFont(mMenuFont);
//   TTF_CloseFont(mFont);
//   TTF_Quit();
//}

//void SdlRenderer::PreRender()
//{

//}

//void SdlRenderer::PostRender()
//{
//   SDL_Flip(mScreen);
//}

//void SdlRenderer::Render(const std::shared_ptr<MenuItem>& obj)
//{
//   const auto pos = obj->GetPosition();

//   SDL_Color color = { 0x90, 0x90, 0x90, 0 };
//   if (obj->IsEnabled()) {
//      color = { 0xC0, 0xC0, 0xC0, 0 };
//   }

//   auto surface = TTF_RenderText_Blended(mMenuFont,
//                                         obj->GetText().c_str(),
//                                         color);
//   SDL_Rect rect = { static_cast<Sint16>(pos.X),
//                     static_cast<Sint16>(pos.Y),
//                     0,
//                     0 };
//   SDL_BlitSurface(surface, NULL, mScreen, &rect);
//   SDL_FreeSurface(surface);
//}

//void SdlRenderer::Render(const std::shared_ptr<MenuItemSelector>& obj)
//{
//   const auto id = obj->GetId();
//   const auto anim_time = obj->GetAnimationTime();
//   const auto frame = mResCache->GetMenuResource(id).GetFrame(anim_time);
//   Render(obj, frame);

////   const auto pos = obj->GetPosition();

////   auto surface = TTF_RenderText_Blended(mMenuFont,
////                                         "Q",
////                                         { 0x70, 0x70, 0x70, 0 });
////   SDL_Rect rect = { static_cast<Sint16>(pos.X),
////                     static_cast<Sint16>(pos.Y),
////                     0,
////                     0 };
////   SDL_BlitSurface(surface, NULL, mScreen, &rect);
////   SDL_FreeSurface(surface);
//}

//void SdlRenderer::Render(const std::shared_ptr<MainMenu>& obj)
//{
//   const auto id = obj->GetId();
//   const auto frame = mResCache->GetMenuResource(id).GetFrame();
//   Render(obj, frame);
//}

//void SdlRenderer::Render(const std::shared_ptr<Arena>& arena)
//{
//   const auto id = arena->GetId();
//   const auto frame = mResCache->GetArenaResource(id).GetFrame();
//   Render(arena, frame);
//}

//void SdlRenderer::Render(const std::shared_ptr<Scoreboard>& scoreboard)
//{
//   const auto pos = scoreboard->GetPosition();
//   const auto size = scoreboard->GetSize();

//   SDL_Rect rect = { static_cast<Sint16>(pos.X),
//                     static_cast<Sint16>(pos.Y),
//                     static_cast<Uint16>(size.Width),
//                     static_cast<Uint16>(size.Height) };
//   SDL_FillRect(mScreen, &rect, 0x1f1f00);

//   const auto line_dist = 20;
//   const auto lines = scoreboard->GetScore();

//   for (size_t i = 0; i < lines.size(); i++)
//   {
//      auto surface = TTF_RenderText_Blended(mFont, lines[i].c_str(), { 0xff,
//                                                                       0xff,
//                                                                       0xff,
//                                                                       0 });
//      SDL_Rect txt_rect = { static_cast<Sint16>(pos.X + 5),
//                            static_cast<Sint16>(pos.Y + 5 + (line_dist * i)),
//                            0,
//                            0 };

//      SDL_BlitSurface(surface, NULL, mScreen, &txt_rect);
//      SDL_FreeSurface(surface);
//   }
//}

//void SdlRenderer::Render(const std::shared_ptr<Wall>& wall)
//{
//   const auto type = wall->GetType();
//   const auto frame = mResCache->GetWallResource(type).GetFrame();
//   Render(wall, frame);
//}

//void SdlRenderer::Render(const std::shared_ptr<Extra>& extra)
//{
//   const auto type = extra->GetType();
//   const auto frame = mResCache->GetExtraResource(type).GetFrame();
//   Render(extra, frame);
//}

//void SdlRenderer::Render(const std::shared_ptr<Bomb>& obj)
//{
//   const auto type = obj->GetType();
//   const auto anim_time = obj->GetAnimationTime();

//   const auto res = mResCache->GetBombResource(type);
//   const auto frame = res.GetFrame(anim_time);
//   Render(obj, frame);
//}

//void SdlRenderer::Render(const std::shared_ptr<Explosion>& obj)
//{
//   const auto type = obj->GetType();
//   const auto anim_time = obj->GetAnimationTime();

//   const auto res = mResCache->GetExplosionResource(type);
//   const auto frame = res.GetFrame(anim_time);
//   Render(obj, frame);
//}

//void SdlRenderer::Render(const std::shared_ptr<Player>& obj)
//{
//   const auto type = obj->GetType();
//   const auto anim_time = obj->GetAnimationTime();

//   const auto res = mResCache->GetPlayerResource(type);
//   const auto frame = res.GetFrame(obj->GetAnimation(),
//                                   anim_time,
//                                   obj->GetSpeed() * obj->GetDistance());

//   auto pos = obj->GetPosition();
//   const auto size = obj->GetSize();

//   pos.X -= (DefaultValue::PLAYER_WIDTH - DefaultValue::CELL_WIDTH) / 2;
//   pos.Y -= (DefaultValue::PLAYER_HEIGHT - DefaultValue::CELL_HEIGHT);

//   SDL_Rect rect = { static_cast<Sint16>(pos.X),
//                     static_cast<Sint16>(pos.Y),
//                     static_cast<Uint16>(size.Width),
//                     static_cast<Uint16>(size.Height) };
//   SDL_BlitSurface(frame, NULL, mScreen, &rect);
//}

//void SdlRenderer::Render(
//   const std::shared_ptr<SceneObject>& obj,
//   SDL_Surface* frame
//)
//{
//   const auto pos = obj->GetPosition();
//   const auto size = obj->GetSize();

//   SDL_Rect rect = { static_cast<Sint16>(pos.X),
//                     static_cast<Sint16>(pos.Y),
//                     static_cast<Uint16>(size.Width),
//                     static_cast<Uint16>(size.Height) };
//   SDL_BlitSurface(frame, NULL, mScreen, &rect);
//}
