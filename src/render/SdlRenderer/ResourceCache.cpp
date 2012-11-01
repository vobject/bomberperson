#include "ResourceCache.hpp"
#include "../../BomberPersonConfig.hpp"
#include "../../game/EntityId.hpp"
#include "../../game/Arena.hpp"
#include "../../game/Wall.hpp"
#include "../../game/Extra.hpp"
#include "../../game/Bomb.hpp"
#include "../../game/Explosion.hpp"
#include "../../game/Player.hpp"
#include "../../utils/Utils.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>

#include <tinyxml.h>

ResourceCache::ResourceCache(const BomberPersonConfig& app_cfg)
   : mAppConfig(app_cfg)
   , mResourceDir(mAppConfig.GetResourceDir() + "/render/SdlRenderer")
   , mResolution(mAppConfig.GetResolution())
{
   if (0 == IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) {
       throw "Failed to initialize SDL_image";
   }

   TiXmlDocument doc(mResourceDir + "/config.xml");
   if (!doc.LoadFile()) {
      throw "Unable to load config XML file.";
   }

   TiXmlHandle doc_hndl(&doc);
   TiXmlHandle root_hndl(doc_hndl.FirstChildElement());
   TiXmlHandle sprite_hndl(root_hndl.FirstChild("Resource").FirstChild("Sprite"));

   LoadMenuResources();
   LoadArenaResources(sprite_hndl.FirstChild("Arena"));
   LoadWallResources(sprite_hndl.FirstChild("Wall"));
   LoadExtraResources(sprite_hndl.FirstChild("Extra"));
   LoadBombResources(sprite_hndl.FirstChild("Bomb"));
   LoadExplosionResources(sprite_hndl.FirstChild("Explosion"));
   LoadPlayerResources(sprite_hndl.FirstChild("Player"));
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

ArenaResource ResourceCache::GetArenaResource(const ArenaType type) const
{
   const auto iter = mArenaRes.find(type);
   if (iter == mArenaRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

WallResource ResourceCache::GetWallResource(const WallType type) const
{
   const auto iter = mWallRes.find(type);
   if (iter == mWallRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

ExtraResource ResourceCache::GetExtraResource(const ExtraType type) const
{
   const auto iter = mExtraRes.find(type);
   if (iter == mExtraRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

BombResource ResourceCache::GetBombResource(const BombType type) const
{
   const auto iter = mBombRes.find(type);
   if (iter == mBombRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

ExplosionResource ResourceCache::GetExplosionResource(const ExplosionType type) const
{
   const auto iter = mExplosionRes.find(type);
   if (iter == mExplosionRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

PlayerResource ResourceCache::GetPlayerResource(const PlayerType type) const
{
   const auto iter = mPlayerRes.find(type);
   if (iter == mPlayerRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

void ResourceCache::LoadMenuResources()
{
   const auto id1 = EntityId::Menu;
   const auto id_sel = EntityId::MenuItemSelector;
   const Size size_sel = { 64, 84 };

   mMenuRes.insert({ id1, { id1, { LoadTexture("mainmenu.png", mResolution) } } });
   mMenuRes.insert({ id_sel, { id_sel,
                               { LoadTexture("menu_selector_1.png", size_sel),
                                 LoadTexture("menu_selector_2.png", size_sel),
                                 LoadTexture("menu_selector_3.png", size_sel),
                                 LoadTexture("menu_selector_4.png", size_sel),
                                 LoadTexture("menu_selector_5.png", size_sel),
                                 LoadTexture("menu_selector_6.png", size_sel) },
                               1000_ms } });
}

void ResourceCache::LoadArenaResources(const TiXmlHandle& hndl)
{
   const Size size = mAppConfig.GetArenaSize();

   ArenaResource arena_1(ArenaType::Arena_1, LoadTextures(hndl, "Arena_1", size));
   ArenaResource arena_2(ArenaType::Arena_2, LoadTextures(hndl, "Arena_2", size));
   ArenaResource arena_3(ArenaType::Arena_3, LoadTextures(hndl, "Arena_3", size));

   mArenaRes.insert({ arena_1.GetType(), arena_1 });
   mArenaRes.insert({ arena_2.GetType(), arena_2 });
   mArenaRes.insert({ arena_3.GetType(), arena_3 });
}

void ResourceCache::LoadWallResources(const TiXmlHandle& hndl)
{
   const Size size = mAppConfig.GetCellSize();

   WallResource indestructible(WallType::Indestructible, LoadTextures(hndl, "Indestructible", size));
   WallResource destructible(WallType::Destructible, LoadTextures(hndl, "Destructible", size));

   mWallRes.insert({ indestructible.GetType(), indestructible });
   mWallRes.insert({ destructible.GetType(), destructible });
}

void ResourceCache::LoadExtraResources(const TiXmlHandle& hndl)
{
   const Size size = mAppConfig.GetCellSize();

   ExtraResource speed(ExtraType::Speed, LoadTextures(hndl, "Speed", size));
   ExtraResource bomb(ExtraType::Bombs, LoadTextures(hndl, "Bombs", size));
   ExtraResource range(ExtraType::Range, LoadTextures(hndl, "Range", size));
   ExtraResource range_gold(ExtraType::InfiniteRange, LoadTextures(hndl, "RangeGold", size));
   ExtraResource kick(ExtraType::Kick, LoadTextures(hndl, "Kick", size));
   ExtraResource remotebombs(ExtraType::RemoteBombs, LoadTextures(hndl, "RemoteBombs", size));

   mExtraRes.insert({ speed.GetType(), speed });
   mExtraRes.insert({ bomb.GetType(), bomb });
   mExtraRes.insert({ range.GetType(), range });
   mExtraRes.insert({ range_gold.GetType(), range_gold });
   mExtraRes.insert({ kick.GetType(), kick });
   mExtraRes.insert({ remotebombs.GetType(), remotebombs });
}

void ResourceCache::LoadBombResources(const TiXmlHandle& hndl)
{
   const Size size = mAppConfig.GetCellSize();
   const auto len = mAppConfig.GetBombLifetime();

   BombResource countdown(BombType::Countdown);
   countdown.SetFrames(len, LoadTextures(hndl, "Countdown", size));

   BombResource remote(BombType::Remote);
   remote.SetFrames(len, LoadTextures(hndl, "Remote", size));

   mBombRes.insert({ countdown.GetType(), countdown });
   mBombRes.insert({ remote.GetType(), remote });
}

void ResourceCache::LoadExplosionResources(const TiXmlHandle& hndl)
{
   const Size size = mAppConfig.GetCellSize();
   const auto len = mAppConfig.GetExplosionLifetime();

   ExplosionResource center(ExplosionType::Center);
   center.SetFrames(len, LoadTextures(hndl, "Center", size));

   ExplosionResource horizontal(ExplosionType::Horizontal);
   horizontal.SetFrames(len, LoadTextures(hndl, "Horizontal", size));

   ExplosionResource horizontal_leftend(ExplosionType::HorizontalLeftEnd);
   horizontal_leftend.SetFrames(len, LoadTextures(hndl, "HorizontalLeftEnd", size));

   ExplosionResource horizontal_rightend(ExplosionType::HorizontalRightEnd);
   horizontal_rightend.SetFrames(len, LoadTextures(hndl, "HorizontalRightEnd", size));

   ExplosionResource vertical(ExplosionType::Vertical);
   vertical.SetFrames(len, LoadTextures(hndl, "Vertical", size));

   ExplosionResource vertical_upend(ExplosionType::VerticalUpEnd);
   vertical_upend.SetFrames(len, LoadTextures(hndl, "VerticalUpEnd", size));

   ExplosionResource vertical_downend(ExplosionType::VerticalDownEnd);
   vertical_downend.SetFrames(len, LoadTextures(hndl, "VerticalDownEnd", size));

   mExplosionRes.insert({ center.GetType(), center });
   mExplosionRes.insert({ horizontal.GetType(), horizontal });
   mExplosionRes.insert({ horizontal_leftend.GetType(), horizontal_leftend });
   mExplosionRes.insert({ horizontal_rightend.GetType(), horizontal_rightend });
   mExplosionRes.insert({ vertical.GetType(), vertical });
   mExplosionRes.insert({ vertical_upend.GetType(), vertical_upend });
   mExplosionRes.insert({ vertical_downend.GetType(), vertical_downend });
}

void ResourceCache::LoadPlayerResources(const TiXmlHandle& hndl)
{
   PlayerResource player_1(LoadPlayer(PlayerType::Player_1, "Player_1", hndl));
   PlayerResource player_2(LoadPlayer(PlayerType::Player_2, "Player_2", hndl));
   PlayerResource player_3(LoadPlayer(PlayerType::Player_3, "Player_3", hndl));
   PlayerResource player_4(LoadPlayer(PlayerType::Player_4, "Player_4", hndl));

   mPlayerRes.insert({ player_1.GetType(), player_1 });
   mPlayerRes.insert({ player_2.GetType(), player_2 });
   mPlayerRes.insert({ player_3.GetType(), player_3 });
   mPlayerRes.insert({ player_4.GetType(), player_4 });
}

PlayerResource ResourceCache::LoadPlayer(
   const PlayerType type,
   const std::string& name,
   const TiXmlHandle& hndl
)
{
   const Size size = { static_cast<int>(mAppConfig.GetCellSize().Width * 1.55f),
                       static_cast<int>(mAppConfig.GetCellSize().Height * 1.75f) };
   const auto walk_len = 1000_ms;
   const auto spawn_len = 1000_ms;
   const auto death_len = 1000_ms;
   const auto player_hndl = hndl.FirstChild(name);

   PlayerResource player(type);
   player.SetFrames(PlayerAnimation::StandUp, walk_len, LoadTextures(player_hndl, "StandUp", size));
   player.SetFrames(PlayerAnimation::StandDown, walk_len, LoadTextures(player_hndl, "StandDown", size));
   player.SetFrames(PlayerAnimation::StandLeft, walk_len, LoadTextures(player_hndl, "StandLeft", size));
   player.SetFrames(PlayerAnimation::StandRight, walk_len, LoadTextures(player_hndl, "StandRight", size));
   player.SetFrames(PlayerAnimation::WalkUp, walk_len, LoadTextures(player_hndl, "WalkUp", size));
   player.SetFrames(PlayerAnimation::WalkDown, walk_len, LoadTextures(player_hndl, "WalkDown", size));
   player.SetFrames(PlayerAnimation::WalkLeft, walk_len, LoadTextures(player_hndl, "WalkLeft", size));
   player.SetFrames(PlayerAnimation::WalkRight, walk_len, LoadTextures(player_hndl, "WalkRight", size));
   player.SetFrames(PlayerAnimation::Spawn, spawn_len, LoadTextures(player_hndl, "Spawn", size));
   player.SetFrames(PlayerAnimation::Destroy, death_len, LoadTextures(player_hndl, "Death", size));
   return player;
}

std::vector<SDL_Surface*> ResourceCache::LoadTextures(
   const TiXmlHandle& hndl,
   const std::string& name,
   const Size size
)
{
   std::vector<SDL_Surface*> textures;

   for (auto elem = hndl.FirstChild(name).Element();
        elem;
        elem = elem->NextSiblingElement(name))
   {
      textures.push_back(LoadTexture(elem->GetText(), size));
   }
   return textures;
}

SDL_Surface* ResourceCache::LoadTexture(const std::string& file, const Size& size)
{
   const auto full_path = mResourceDir + "/sprite/" + file;

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
