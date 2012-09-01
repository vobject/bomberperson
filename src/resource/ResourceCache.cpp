#include "ResourceCache.hpp"
#include "../game/EntityId.hpp"
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
   LoadMenuResources();
   LoadArenaResources();
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

SpriteResource ResourceCache::GetMenuResource(const EntityId id) const
{
   const auto iter = mMenuRes.find(id);
   if (iter == mMenuRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

SpriteResource ResourceCache::GetArenaResource(const EntityId id) const
{
   const auto iter = mArenaRes.find(id);
   if (iter == mArenaRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

SpriteResource ResourceCache::GetWallResource(const EntityId id) const
{
   const auto iter = mWallRes.find(id);
   if (iter == mWallRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

SpriteResource ResourceCache::GetExtraResource(const EntityId id) const
{
   const auto iter = mExtraRes.find(id);
   if (iter == mExtraRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

SpriteResource ResourceCache::GetBombResource(const EntityId id) const
{
   const auto iter = mBombRes.find(id);
   if (iter == mBombRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

SpriteResource ResourceCache::GetExplosionResource(const EntityId id) const
{
   const auto iter = mExplosionRes.find(id);
   if (iter == mExplosionRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

PlayerResource ResourceCache::GetPlayerResource(const EntityId id) const
{
   const auto iter = mPlayerRes.find(id);
   if (iter == mPlayerRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

void ResourceCache::LoadMenuResources()
{
   const Size size = { DefaultSize::SCREEN_WIDTH,
                       DefaultSize::SCREEN_HEIGHT };

   mMenuRes[EntityId::MainMenu] = {
      EntityId::MainMenu,
      { LoadTexture("sprite/mainmenu.png", size) }
   };
}

void ResourceCache::LoadArenaResources()
{
   const Size size = { DefaultSize::ARENA_WIDTH,
                       DefaultSize::ARENA_HEIGHT };

   mArenaRes[EntityId::Arena] = {
      EntityId::Arena,
      { LoadTexture("sprite/arena_1.png", size) }
   };
}

void ResourceCache::LoadWallResources()
{
   const Size size = { DefaultSize::WALL_WIDTH,
                       DefaultSize::WALL_HEIGHT };

   mWallRes[EntityId::IndestructibleWall] = {
      EntityId::IndestructibleWall,
      { LoadTexture("sprite/wall_indestructible.png", size) }
   };

   mWallRes[EntityId::DestructibleWall] = {
      EntityId::DestructibleWall,
      { LoadTexture("sprite/wall_destructible.png", size) }
   };
}

void ResourceCache::LoadExtraResources()
{
   const Size size = { DefaultSize::EXTRA_WIDTH,
                       DefaultSize::EXTRA_HEIGHT };

   mExtraRes[EntityId::SpeedExtra] = {
      EntityId::SpeedExtra,
      { LoadTexture("sprite/extra_speed.png", size) }
   };

   mExtraRes[EntityId::BombsExtra] = {
      EntityId::BombsExtra,
      { LoadTexture("sprite/extra_supply.png", size) }
   };

   mExtraRes[EntityId::RangeExtra] = {
      EntityId::RangeExtra,
      { LoadTexture("sprite/extra_range.png", size) }
   };

   mExtraRes[EntityId::GoldRangeExtra] = {
      EntityId::GoldRangeExtra,
      { LoadTexture("sprite/extra_range_gold.png", size) }
   };
}

void ResourceCache::LoadBombResources()
{
   const Size size = { DefaultSize::BOMB_WIDTH,
                       DefaultSize::BOMB_HEIGHT };

   mBombRes[EntityId::Bomb] = {
      EntityId::Bomb,
      { LoadTexture("sprite/bomb_1.png", size),
        LoadTexture("sprite/bomb_2.png", size),
        LoadTexture("sprite/bomb_3.png", size) }
   };
}

void ResourceCache::LoadExplosionResources()
{
   const Size size = { DefaultSize::EXPLOSION_WIDTH,
                       DefaultSize::EXPLOSION_HEIGHT };

   mExplosionRes[EntityId::Explosion] = {
      EntityId::Explosion,
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

   PlayerResource player_1(EntityId::Player_1);
   player_1.SetWalkFrames(Direction::Up,
                          { LoadTexture("sprite/player_1_up_1.png", size),
                            LoadTexture("sprite/player_1_up_2.png", size) });
   player_1.SetWalkFrames(Direction::Down,
                          { LoadTexture("sprite/player_1_down_1.png", size),
                            LoadTexture("sprite/player_1_down_2.png", size) });
   player_1.SetWalkFrames(Direction::Left,
                          { LoadTexture("sprite/player_1_left_1.png", size),
                            LoadTexture("sprite/player_1_left_2.png", size) });
   player_1.SetWalkFrames(Direction::Right,
                          { LoadTexture("sprite/player_1_right_1.png", size),
                            LoadTexture("sprite/player_1_right_2.png", size) });
   mPlayerRes[player_1.GetId()] = player_1;

   PlayerResource player_2(EntityId::Player_2);
   player_2.SetWalkFrames(Direction::Up,
                          { LoadTexture("sprite/player_2_up_1.png", size),
                            LoadTexture("sprite/player_2_up_2.png", size) });
   player_2.SetWalkFrames(Direction::Down,
                          { LoadTexture("sprite/player_2_down_1.png", size),
                            LoadTexture("sprite/player_2_down_2.png", size) });
   player_2.SetWalkFrames(Direction::Left,
                          { LoadTexture("sprite/player_2_left_1.png", size),
                            LoadTexture("sprite/player_2_left_2.png", size) });
   player_2.SetWalkFrames(Direction::Right,
                          { LoadTexture("sprite/player_2_right_1.png", size),
                            LoadTexture("sprite/player_2_right_2.png", size) });
   mPlayerRes[player_2.GetId()] = player_2;

   PlayerResource player_3(EntityId::Player_3);
   player_3.SetWalkFrames(Direction::Up,
                          { LoadTexture("sprite/player_3_up_1.png", size),
                            LoadTexture("sprite/player_3_up_2.png", size) });
   player_3.SetWalkFrames(Direction::Down,
                          { LoadTexture("sprite/player_3_down_1.png", size),
                            LoadTexture("sprite/player_3_down_2.png", size) });
   player_3.SetWalkFrames(Direction::Left,
                          { LoadTexture("sprite/player_3_left_1.png", size),
                            LoadTexture("sprite/player_3_left_2.png", size) });
   player_3.SetWalkFrames(Direction::Right,
                          { LoadTexture("sprite/player_3_right_1.png", size),
                            LoadTexture("sprite/player_3_right_2.png", size) });
   mPlayerRes[player_3.GetId()] = player_3;

   PlayerResource player_4(EntityId::Player_4);
   player_4.SetWalkFrames(Direction::Up,
                          { LoadTexture("sprite/player_4_up_1.png", size),
                            LoadTexture("sprite/player_4_up_2.png", size) });
   player_4.SetWalkFrames(Direction::Down,
                          { LoadTexture("sprite/player_4_down_1.png", size),
                            LoadTexture("sprite/player_4_down_2.png", size) });
   player_4.SetWalkFrames(Direction::Left,
                          { LoadTexture("sprite/player_4_left_1.png", size),
                            LoadTexture("sprite/player_4_left_2.png", size) });
   player_4.SetWalkFrames(Direction::Right,
                          { LoadTexture("sprite/player_4_right_1.png", size),
                            LoadTexture("sprite/player_4_right_2.png", size) });
   mPlayerRes[player_4.GetId()] = player_4;
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

   const auto colorkey = SDL_MapRGB(img_zoomed->format, 0, 0, 0);
   if (SDL_SetColorKey(img_zoomed, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkey)) {
      throw "SDL_SetColorKey failed";
   }

   mSurfaceCache.push_back(img_zoomed);
   return img_zoomed;
}
