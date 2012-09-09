#include "ResourceCache.hpp"
#include "../game/EntityId.hpp"
#include "../game/Player.hpp"
#include "../utils/Utils.hpp"
#include "../Options.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>

ResourceCache::ResourceCache()
   : mResDir("res")
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
   const auto id = EntityId::MainMenu;
   const Size size = { DefaultSize::SCREEN_WIDTH,
                       DefaultSize::SCREEN_HEIGHT };

   mMenuRes.insert({ id, {id, { LoadTexture("sprite/mainmenu.png", size) } } });
}

void ResourceCache::LoadArenaResources()
{
   const auto id = EntityId::Arena;
   const Size size = { DefaultSize::ARENA_WIDTH,
                       DefaultSize::ARENA_HEIGHT };

   mArenaRes.insert({ id, {id, { LoadTexture("sprite/arena_1.png", size) } } });
}

void ResourceCache::LoadWallResources()
{
   const auto id1 = EntityId::IndestructibleWall;
   const auto id2 = EntityId::DestructibleWall;
   const Size size = { DefaultSize::WALL_WIDTH,
                       DefaultSize::WALL_HEIGHT };

   mWallRes.insert({ id1, {id1, { LoadTexture("sprite/wall_indestructible.png", size) } } });
   mWallRes.insert({ id2, {id2, { LoadTexture("sprite/wall_destructible.png", size) } } });
}

void ResourceCache::LoadExtraResources()
{
   const auto id1 = EntityId::SpeedExtra;
   const auto id2 = EntityId::BombsExtra;
   const auto id3 = EntityId::RangeExtra;
   const auto id4 = EntityId::GoldRangeExtra;
   const Size size = { DefaultSize::EXTRA_WIDTH,
                       DefaultSize::EXTRA_HEIGHT };

   mExtraRes.insert({ id1, {id1, { LoadTexture("sprite/extra_speed.png", size) } } });
   mExtraRes.insert({ id2, {id2, { LoadTexture("sprite/extra_supply.png", size) } } });
   mExtraRes.insert({ id3, {id3, { LoadTexture("sprite/extra_range.png", size) } } });
   mExtraRes.insert({ id4, {id4, { LoadTexture("sprite/extra_range_gold.png", size) } } });
}

void ResourceCache::LoadBombResources()
{
   const auto id = EntityId::Bomb;
   const Size size = { DefaultSize::BOMB_WIDTH,
                       DefaultSize::BOMB_HEIGHT };

   // TODO: Align animation speed and bomb lifetime.
   mBombRes.insert({
      id,
      { id,
        { LoadTexture("sprite/bomb_1.png", size),
          LoadTexture("sprite/bomb_2.png", size),
          LoadTexture("sprite/bomb_3.png", size) },
         2500_ms,
         false
      }
   });
}

void ResourceCache::LoadExplosionResources()
{
   const auto id = EntityId::Explosion;
   const Size size = { DefaultSize::EXPLOSION_WIDTH,
                       DefaultSize::EXPLOSION_HEIGHT };

   // TODO: Align animation speed and explosion lifetime.
   mExplosionRes.insert({
      id,
      { id,
        { LoadTexture("sprite/explosion_1.png", size),
          LoadTexture("sprite/explosion_2.png", size),
          LoadTexture("sprite/explosion_3.png", size),
          LoadTexture("sprite/explosion_4.png", size) },
         1000_ms,
         false
      }
   });
}

void ResourceCache::LoadPlayerResources()
{
   const Size size = { DefaultSize::PLAYER_WIDTH,
                       DefaultSize::PLAYER_HEIGHT };

   // TODO: align player speed and animation speed!!
   PlayerResource player_1(EntityId::Player_1, 2000_ms);
   player_1.SetFrames(PlayerAnimation::StandUp, { LoadTexture("sprite/player_1_up.png", size) });
   player_1.SetFrames(PlayerAnimation::StandDown, { LoadTexture("sprite/player_1_down.png", size) });
   player_1.SetFrames(PlayerAnimation::StandLeft, { LoadTexture("sprite/player_1_left.png", size) });
   player_1.SetFrames(PlayerAnimation::StandRight, { LoadTexture("sprite/player_1_right.png", size) });
   player_1.SetFrames(PlayerAnimation::WalkUp, { LoadTexture("sprite/player_1_up_1.png", size), LoadTexture("sprite/player_1_up_2.png", size) });
   player_1.SetFrames(PlayerAnimation::WalkDown, { LoadTexture("sprite/player_1_down_1.png", size), LoadTexture("sprite/player_1_down_2.png", size) });
   player_1.SetFrames(PlayerAnimation::WalkLeft, { LoadTexture("sprite/player_1_left_1.png", size), LoadTexture("sprite/player_1_left_2.png", size) });
   player_1.SetFrames(PlayerAnimation::WalkRight, { LoadTexture("sprite/player_1_right_1.png", size), LoadTexture("sprite/player_1_right_2.png", size) });
   mPlayerRes.insert({ player_1.GetId(), player_1 });

   PlayerResource player_2(EntityId::Player_2, 2000_ms);
   player_2.SetFrames(PlayerAnimation::StandUp, { LoadTexture("sprite/player_2_up.png", size) });
   player_2.SetFrames(PlayerAnimation::StandDown, { LoadTexture("sprite/player_2_down.png", size) });
   player_2.SetFrames(PlayerAnimation::StandLeft, { LoadTexture("sprite/player_2_left.png", size) });
   player_2.SetFrames(PlayerAnimation::StandRight, { LoadTexture("sprite/player_2_right.png", size) });
   player_2.SetFrames(PlayerAnimation::WalkUp, { LoadTexture("sprite/player_2_up_1.png", size), LoadTexture("sprite/player_2_up_2.png", size) });
   player_2.SetFrames(PlayerAnimation::WalkDown, { LoadTexture("sprite/player_2_down_1.png", size), LoadTexture("sprite/player_2_down_2.png", size) });
   player_2.SetFrames(PlayerAnimation::WalkLeft, { LoadTexture("sprite/player_2_left_1.png", size), LoadTexture("sprite/player_2_left_2.png", size) });
   player_2.SetFrames(PlayerAnimation::WalkRight, { LoadTexture("sprite/player_2_right_1.png", size), LoadTexture("sprite/player_2_right_2.png", size) });
   mPlayerRes.insert({ player_2.GetId(), player_2 });

   PlayerResource player_3(EntityId::Player_3, 2000_ms);
   player_3.SetFrames(PlayerAnimation::StandUp, { LoadTexture("sprite/player_3_up.png", size) });
   player_3.SetFrames(PlayerAnimation::StandDown, { LoadTexture("sprite/player_3_down.png", size) });
   player_3.SetFrames(PlayerAnimation::StandLeft, { LoadTexture("sprite/player_3_left.png", size) });
   player_3.SetFrames(PlayerAnimation::StandRight, { LoadTexture("sprite/player_3_right.png", size) });
   player_3.SetFrames(PlayerAnimation::WalkUp, { LoadTexture("sprite/player_3_up_1.png", size), LoadTexture("sprite/player_3_up_2.png", size) });
   player_3.SetFrames(PlayerAnimation::WalkDown, { LoadTexture("sprite/player_3_down_1.png", size), LoadTexture("sprite/player_3_down_2.png", size) });
   player_3.SetFrames(PlayerAnimation::WalkLeft, { LoadTexture("sprite/player_3_left_1.png", size), LoadTexture("sprite/player_3_left_2.png", size) });
   player_3.SetFrames(PlayerAnimation::WalkRight, { LoadTexture("sprite/player_3_right_1.png", size), LoadTexture("sprite/player_3_right_2.png", size) });
   mPlayerRes.insert({ player_3.GetId(), player_3 });

   PlayerResource player_4(EntityId::Player_4, 2000_ms);
   player_4.SetFrames(PlayerAnimation::StandUp, { LoadTexture("sprite/player_4_up.png", size) });
   player_4.SetFrames(PlayerAnimation::StandDown, { LoadTexture("sprite/player_4_down.png", size) });
   player_4.SetFrames(PlayerAnimation::StandLeft, { LoadTexture("sprite/player_4_left.png", size) });
   player_4.SetFrames(PlayerAnimation::StandRight, { LoadTexture("sprite/player_4_right.png", size) });
   player_4.SetFrames(PlayerAnimation::WalkUp, { LoadTexture("sprite/player_4_up_1.png", size), LoadTexture("sprite/player_4_up_2.png", size) });
   player_4.SetFrames(PlayerAnimation::WalkDown, { LoadTexture("sprite/player_4_down_1.png", size), LoadTexture("sprite/player_4_down_2.png", size) });
   player_4.SetFrames(PlayerAnimation::WalkLeft, { LoadTexture("sprite/player_4_left_1.png", size), LoadTexture("sprite/player_4_left_2.png", size) });
   player_4.SetFrames(PlayerAnimation::WalkRight, { LoadTexture("sprite/player_4_right_1.png", size), LoadTexture("sprite/player_4_right_2.png", size) });
   mPlayerRes.insert({ player_4.GetId(), player_4 });
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
