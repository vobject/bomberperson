#include "ResourceCache.hpp"
#include "../game/Player.hpp"
#include "../utils/Utils.hpp"
#include "../Options.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>

ResourceCache::ResourceCache()
   : mResDir("res_nonfree")
{
   if (0 == IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) {
       throw "Failed to initialize SDL_image";
   }

   // TODO: Coordinate this using XML files.
   LoadBackgroundResources();
   LoadWallResources();
   LoadExtraResources();
   LoadBombResources();
   LoadExplosionResources();
   LoadPlayerResources();
}

ResourceCache::~ResourceCache()
{
   for (auto& surface : mSurfaceCache) {
      SDL_FreeSurface(surface);
   }
}

SpriteResource ResourceCache::GetBgResource(const std::string& id) const
{
   const auto iter = mBackgroundRes.find(id);
   if (iter == mBackgroundRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

SpriteResource ResourceCache::GetWallResource(const std::string& id) const
{
   const auto iter = mWallRes.find(id);
   if (iter == mWallRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

SpriteResource ResourceCache::GetExtraResource(const std::string& id) const
{
   const auto iter = mExtraRes.find(id);
   if (iter == mExtraRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

SpriteResource ResourceCache::GetBombResource(const std::string& id) const
{
   const auto iter = mBombRes.find(id);
   if (iter == mBombRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

SpriteResource ResourceCache::GetExplosionResource(const std::string& id) const
{
   const auto iter = mExplosionRes.find(id);
   if (iter == mExplosionRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

PlayerResource ResourceCache::GetPlayerResource(const std::string& id) const
{
   const auto iter = mPlayerRes.find(id);
   if (iter == mPlayerRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

void ResourceCache::LoadBackgroundResources()
{
   const Size size = { DefaultSize::ARENA_BG_WIDTH,
                       DefaultSize::ARENA_BG_HEIGHT };

   mBackgroundRes["bg_arena_1"] = {
      "bg_arena_1",
      { LoadTexture("sprite/bg_arena_1.png", size) }
   };
}

void ResourceCache::LoadWallResources()
{
   const Size size = { DefaultSize::WALL_WIDTH,
                       DefaultSize::WALL_HEIGHT };

   mWallRes["wall_destructible"] = {
      "wall_destructible",
      { LoadTexture("sprite/wall_destructible.png", size) }
   };

   mWallRes["wall_indestructible"] = {
      "wall_indestructible",
      { LoadTexture("sprite/wall_indestructible.png", size) }
   };
}

void ResourceCache::LoadExtraResources()
{
   const Size size = { DefaultSize::EXTRA_WIDTH,
                       DefaultSize::EXTRA_HEIGHT };

   mExtraRes["extra_speed"] = {
      "extra_speed",
      { LoadTexture("sprite/extra_speed.png", size) }
   };

   mExtraRes["extra_bombs"] = {
      "extra_bombs",
      { LoadTexture("sprite/extra_supply.png", size) }
   };

   mExtraRes["extra_range"] = {
      "extra_range",
      { LoadTexture("sprite/extra_range.png", size) }
   };
}

void ResourceCache::LoadBombResources()
{
   const Size size = { DefaultSize::BOMB_WIDTH,
                       DefaultSize::BOMB_HEIGHT };

   mBombRes["bomb"] = {
      "bomb",
      { LoadTexture("sprite/bomb_1.png", size),
        LoadTexture("sprite/bomb_2.png", size),
        LoadTexture("sprite/bomb_3.png", size) }
   };
}

void ResourceCache::LoadExplosionResources()
{
   const Size size = { DefaultSize::EXPLOSION_WIDTH,
                       DefaultSize::EXPLOSION_HEIGHT };

   mExplosionRes["explosion"] = {
      "explosion",
      { LoadTexture("sprite/explosion_1.png", size),
        LoadTexture("sprite/explosion_2.png", size),
        LoadTexture("sprite/explosion_3.png", size),
        LoadTexture("sprite/explosion_4.png", size) }
   };
}

void ResourceCache::LoadPlayerResources()
{
   const Size size = { DefaultSize::PLAYER_WIDTH,
                       DefaultSize::PLAYER_HEIGHT };

   PlayerResource player_1("player_1");
   player_1.SetFrames(PlayerState::StandUp, { LoadTexture("sprite/player_1_up.png", size) });
   player_1.SetFrames(PlayerState::StandDown, { LoadTexture("sprite/player_1_down.png", size) });
   player_1.SetFrames(PlayerState::StandLeft, { LoadTexture("sprite/player_1_left.png", size) });
   player_1.SetFrames(PlayerState::StandRight, { LoadTexture("sprite/player_1_right.png", size) });
   player_1.SetFrames(PlayerState::WalkUp, { LoadTexture("sprite/player_1_up_1.png", size), LoadTexture("sprite/player_1_up_2.png", size) });
   player_1.SetFrames(PlayerState::WalkDown, { LoadTexture("sprite/player_1_down_1.png", size), LoadTexture("sprite/player_1_down_2.png", size) });
   player_1.SetFrames(PlayerState::WalkLeft, { LoadTexture("sprite/player_1_left_1.png", size), LoadTexture("sprite/player_1_left_2.png", size) });
   player_1.SetFrames(PlayerState::WalkRight, { LoadTexture("sprite/player_1_right_1.png", size), LoadTexture("sprite/player_1_right_2.png", size) });
   player_1.SetAnimationLength(2000); // TODO: align player speed and animation speed!!
   mPlayerRes[player_1.GetId()] = player_1;

//   PlayerResource player_2("player_2");
//   player_2.SetWalkFrames(Direction::Up,
//                          { LoadTexture("sprite/player_2_up_1.png", size),
//                            LoadTexture("sprite/player_2_up_2.png", size) });
//   player_2.SetWalkFrames(Direction::Down,
//                          { LoadTexture("sprite/player_2_down_1.png", size),
//                            LoadTexture("sprite/player_2_down_2.png", size) });
//   player_2.SetWalkFrames(Direction::Left,
//                          { LoadTexture("sprite/player_2_left_1.png", size),
//                            LoadTexture("sprite/player_2_left_2.png", size) });
//   player_2.SetWalkFrames(Direction::Right,
//                          { LoadTexture("sprite/player_2_right_1.png", size),
//                            LoadTexture("sprite/player_2_right_2.png", size) });
//   mPlayerRes[player_2.GetId()] = player_2;

//   PlayerResource player_3("player_3");
//   player_3.SetWalkFrames(Direction::Up,
//                          { LoadTexture("sprite/player_3_up_1.png", size),
//                            LoadTexture("sprite/player_3_up_2.png", size) });
//   player_3.SetWalkFrames(Direction::Down,
//                          { LoadTexture("sprite/player_3_down_1.png", size),
//                            LoadTexture("sprite/player_3_down_2.png", size) });
//   player_3.SetWalkFrames(Direction::Left,
//                          { LoadTexture("sprite/player_3_left_1.png", size),
//                            LoadTexture("sprite/player_3_left_2.png", size) });
//   player_3.SetWalkFrames(Direction::Right,
//                          { LoadTexture("sprite/player_3_right_1.png", size),
//                            LoadTexture("sprite/player_3_right_2.png", size) });
//   mPlayerRes[player_3.GetId()] = player_3;

//   PlayerResource player_4("player_4");
//   player_4.SetWalkFrames(Direction::Up,
//                          { LoadTexture("sprite/player_4_up_1.png", size),
//                            LoadTexture("sprite/player_4_up_2.png", size) });
//   player_4.SetWalkFrames(Direction::Down,
//                          { LoadTexture("sprite/player_4_down_1.png", size),
//                            LoadTexture("sprite/player_4_down_2.png", size) });
//   player_4.SetWalkFrames(Direction::Left,
//                          { LoadTexture("sprite/player_4_left_1.png", size),
//                            LoadTexture("sprite/player_4_left_2.png", size) });
//   player_4.SetWalkFrames(Direction::Right,
//                          { LoadTexture("sprite/player_4_right_1.png", size),
//                            LoadTexture("sprite/player_4_right_2.png", size) });
//   mPlayerRes[player_4.GetId()] = player_4;
}

SDL_Surface* ResourceCache::LoadTexture(const std::string& file, const Size& size)
{
   const auto full_path = mResDir + "/" + file;

   SDL_Surface* img_loaded = IMG_Load(full_path.c_str());
   if (!img_loaded) {
      throw "Failed to load texture";
   }

   SDL_Surface* img_compat = SDL_DisplayFormat(img_loaded);
   if (!img_compat) {
      throw "Failed to convert animation frame to display format";
   }
   SDL_FreeSurface(img_loaded);
   img_loaded = nullptr;

   const auto x_zoom = static_cast<double>(size.Width) / img_compat->w;
   const auto y_zoom = static_cast<double>(size.Height) / img_compat->h;
   const auto img_zoomed = zoomSurface(img_compat, x_zoom, y_zoom, 0);
   SDL_FreeSurface(img_compat);
   img_compat = nullptr;

   const auto colorkey = SDL_MapRGB(img_zoomed->format, 0xff, 0, 0xff);
   if (SDL_SetColorKey(img_zoomed, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey)) {
      throw "SDL_SetColorKey failed";
   }

   mSurfaceCache.push_back(img_zoomed);
   return img_zoomed;
}
