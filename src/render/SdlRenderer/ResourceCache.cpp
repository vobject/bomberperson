#include "ResourceCache.hpp"
#include "../../game/EntityId.hpp"
#include "../../game/Wall.hpp"
#include "../../game/Extra.hpp"
#include "../../game/Bomb.hpp"
#include "../../game/Explosion.hpp"
#include "../../game/Player.hpp"
#include "../../utils/Utils.hpp"
#include "../../Options.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>

#include <tinyxml.h>

ResourceCache::ResourceCache(const std::string& renderer_dir)
   : mResDir(renderer_dir)
{
   if (0 == IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) {
       throw "Failed to initialize SDL_image";
   }

   TiXmlDocument doc(mResDir + "/config.xml");
   if (!doc.LoadFile()) {
      throw "Unable to load config XML file.";
   }

   TiXmlHandle doc_hndl(&doc);
   TiXmlHandle root_hndl(doc_hndl.FirstChildElement());
   TiXmlHandle sprite_hndl(root_hndl.FirstChild("Resource").FirstChild("Sprite"));

   LoadMenuResources();
   LoadArenaResources();
   LoadWallResources(sprite_hndl.FirstChild("Wall"));
   LoadExtraResources(sprite_hndl.FirstChild("Extra"));
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
   const Size size = { DefaultValue::SCREEN_WIDTH, DefaultValue::SCREEN_HEIGHT };
   const Size size_sel = { 64, 84 };

   mMenuRes.insert({ id1, { id1, { LoadTexture("mainmenu.png", size) } } });
   mMenuRes.insert({ id_sel, { id_sel,
                               { LoadTexture("menu_selector_1.png", size_sel),
                                 LoadTexture("menu_selector_2.png", size_sel),
                                 LoadTexture("menu_selector_3.png", size_sel),
                                 LoadTexture("menu_selector_4.png", size_sel),
                                 LoadTexture("menu_selector_5.png", size_sel),
                                 LoadTexture("menu_selector_6.png", size_sel) },
                               1000_ms } });
}

void ResourceCache::LoadArenaResources()
{
   const auto id = EntityId::Arena;
   const Size size = { DefaultValue::ARENA_WIDTH, DefaultValue::ARENA_HEIGHT };

   mArenaRes.insert({ id, { id, { LoadTexture("arena_1.png", size) } } });
}

void ResourceCache::LoadWallResources(const TiXmlHandle& hndl)
{
   const Size size = { DefaultValue::WALL_WIDTH, DefaultValue::WALL_HEIGHT };

   const auto i_file = hndl.FirstChild("Indestructible").Element()->GetText();
   const auto d_file = hndl.FirstChild("Destructible").Element()->GetText();

   mWallRes.insert({ WallType::Indestructible, { WallType::Indestructible, { LoadTexture(i_file, size) } } });
   mWallRes.insert({ WallType::Destructible, { WallType::Destructible, { LoadTexture(d_file, size) } } });
}

void ResourceCache::LoadExtraResources(const TiXmlHandle& hndl)
{
   const Size size = { DefaultValue::EXTRA_WIDTH, DefaultValue::EXTRA_HEIGHT };

   const auto speed_file = hndl.FirstChild("Speed").Element()->GetText();
   const auto bombs_file = hndl.FirstChild("Bombs").Element()->GetText();
   const auto range_file = hndl.FirstChild("Range").Element()->GetText();
   const auto rangegold_file = hndl.FirstChild("RangeGold").Element()->GetText();
   const auto kick_file = hndl.FirstChild("Kick").Element()->GetText();
   const auto remotebombs_file = hndl.FirstChild("RemoteBombs").Element()->GetText();

   mExtraRes.insert({ ExtraType::Speed, { ExtraType::Speed, { LoadTexture(speed_file, size) } } });
   mExtraRes.insert({ ExtraType::Bombs, { ExtraType::Bombs, { LoadTexture(bombs_file, size) } } });
   mExtraRes.insert({ ExtraType::Range, { ExtraType::Range, { LoadTexture(range_file, size) } } });
   mExtraRes.insert({ ExtraType::InfiniteRange, { ExtraType::InfiniteRange, { LoadTexture(rangegold_file, size) } } });
   mExtraRes.insert({ ExtraType::Kick, { ExtraType::Kick, { LoadTexture(kick_file, size) } } });
   mExtraRes.insert({ ExtraType::RemoteBombs, { ExtraType::RemoteBombs, { LoadTexture(remotebombs_file, size) } } });
}

void ResourceCache::LoadBombResources()
{
   const Size size = { DefaultValue::BOMB_WIDTH, DefaultValue::BOMB_HEIGHT };
   const auto len = DefaultValue::BOMB_ANIM_LEN;

   BombResource countdown(BombType::Countdown);
   countdown.SetFrames(len, { LoadTexture("bomb_1.png", size), LoadTexture("bomb_2.png", size), LoadTexture("bomb_3.png", size) });
   mBombRes.insert({ countdown.GetType(), countdown });

   BombResource remote(BombType::Remote);
   remote.SetFrames(len, { LoadTexture("bomb_remote_1.png", size), LoadTexture("bomb_remote_2.png", size) });
   mBombRes.insert({ remote.GetType(), remote });
}

void ResourceCache::LoadExplosionResources()
{
   const Size size = { DefaultValue::EXPLOSION_WIDTH, DefaultValue::EXPLOSION_HEIGHT };
   const auto len = DefaultValue::EXPLOSION_ANIM_LEN;

   ExplosionResource center(ExplosionType::Center);
   center.SetFrames(len, { LoadTexture("explosion_center_1.png", size), LoadTexture("explosion_center_2.png", size), LoadTexture("explosion_center_3.png", size), LoadTexture("explosion_center_4.png", size), LoadTexture("explosion_center_3.png", size), LoadTexture("explosion_center_2.png", size), LoadTexture("explosion_center_1.png", size) });
   mExplosionRes.insert({ center.GetType(), center });

   ExplosionResource horizontal(ExplosionType::Horizontal);
   horizontal.SetFrames(len, { LoadTexture("explosion_horizontal_1.png", size), LoadTexture("explosion_horizontal_2.png", size), LoadTexture("explosion_horizontal_3.png", size), LoadTexture("explosion_horizontal_4.png", size), LoadTexture("explosion_horizontal_3.png", size), LoadTexture("explosion_horizontal_2.png", size), LoadTexture("explosion_horizontal_1.png", size) });
   mExplosionRes.insert({ horizontal.GetType(), horizontal });

   ExplosionResource horizontal_leftend(ExplosionType::HorizontalLeftEnd);
   horizontal_leftend.SetFrames(len, { LoadTexture("explosion_horizontal_leftend_1.png", size), LoadTexture("explosion_horizontal_leftend_2.png", size), LoadTexture("explosion_horizontal_leftend_3.png", size), LoadTexture("explosion_horizontal_leftend_4.png", size), LoadTexture("explosion_horizontal_leftend_3.png", size), LoadTexture("explosion_horizontal_leftend_2.png", size), LoadTexture("explosion_horizontal_leftend_1.png", size) });
   mExplosionRes.insert({ horizontal_leftend.GetType(), horizontal_leftend });

   ExplosionResource horizontal_rightend(ExplosionType::HorizontalRightEnd);
   horizontal_rightend.SetFrames(len, { LoadTexture("explosion_horizontal_rightend_1.png", size), LoadTexture("explosion_horizontal_rightend_2.png", size), LoadTexture("explosion_horizontal_rightend_3.png", size), LoadTexture("explosion_horizontal_rightend_4.png", size), LoadTexture("explosion_horizontal_rightend_3.png", size), LoadTexture("explosion_horizontal_rightend_2.png", size), LoadTexture("explosion_horizontal_rightend_1.png", size) });
   mExplosionRes.insert({ horizontal_rightend.GetType(), horizontal_rightend });

   ExplosionResource vertical(ExplosionType::Vertical);
   vertical.SetFrames(len, { LoadTexture("explosion_vertical_1.png", size), LoadTexture("explosion_vertical_2.png", size), LoadTexture("explosion_vertical_3.png", size), LoadTexture("explosion_vertical_4.png", size), LoadTexture("explosion_vertical_3.png", size), LoadTexture("explosion_vertical_2.png", size), LoadTexture("explosion_vertical_1.png", size) });
   mExplosionRes.insert({ vertical.GetType(), vertical });

   ExplosionResource vertical_upend(ExplosionType::VerticalUpEnd);
   vertical_upend.SetFrames(len, { LoadTexture("explosion_vertical_upend_1.png", size), LoadTexture("explosion_vertical_upend_2.png", size), LoadTexture("explosion_vertical_upend_3.png", size), LoadTexture("explosion_vertical_upend_4.png", size), LoadTexture("explosion_vertical_upend_3.png", size), LoadTexture("explosion_vertical_upend_2.png", size), LoadTexture("explosion_vertical_upend_1.png", size) });
   mExplosionRes.insert({ vertical_upend.GetType(), vertical_upend });

   ExplosionResource vertical_downend(ExplosionType::VerticalDownEnd);
   vertical_downend.SetFrames(len, { LoadTexture("explosion_vertical_downend_1.png", size), LoadTexture("explosion_vertical_downend_2.png", size), LoadTexture("explosion_vertical_downend_3.png", size), LoadTexture("explosion_vertical_downend_4.png", size), LoadTexture("explosion_vertical_downend_3.png", size), LoadTexture("explosion_vertical_downend_2.png", size), LoadTexture("explosion_vertical_downend_1.png", size) });
   mExplosionRes.insert({ vertical_downend.GetType(), vertical_downend });
}

void ResourceCache::LoadPlayerResources()
{
   const Size size = { DefaultValue::PLAYER_WIDTH,
                       DefaultValue::PLAYER_HEIGHT };
   const auto walk_len = DefaultValue::PLAYER_WALK_ANIM_LEN;
   const auto spawn_len = DefaultValue::PLAYER_SPAWN_ANIM_LEN;
   const auto death_len = DefaultValue::PLAYER_DEATH_ANIM_LEN;

   PlayerResource player_1(PlayerType::Player_1);
   player_1.SetFrames(PlayerAnimation::StandUp, walk_len, { LoadTexture("player_1_up.png", size) });
   player_1.SetFrames(PlayerAnimation::StandDown, walk_len, { LoadTexture("player_1_down.png", size) });
   player_1.SetFrames(PlayerAnimation::StandLeft, walk_len, { LoadTexture("player_1_left.png", size) });
   player_1.SetFrames(PlayerAnimation::StandRight, walk_len, { LoadTexture("player_1_right.png", size) });
   player_1.SetFrames(PlayerAnimation::WalkUp, walk_len, { LoadTexture("player_1_up_1.png", size), LoadTexture("player_1_up_2.png", size) });
   player_1.SetFrames(PlayerAnimation::WalkDown, walk_len, { LoadTexture("player_1_down_1.png", size), LoadTexture("player_1_down_2.png", size) });
   player_1.SetFrames(PlayerAnimation::WalkLeft, walk_len, { LoadTexture("player_1_left_1.png", size), LoadTexture("player_1_left_2.png", size) });
   player_1.SetFrames(PlayerAnimation::WalkRight, walk_len, { LoadTexture("player_1_right_1.png", size), LoadTexture("player_1_right_2.png", size) });
   player_1.SetFrames(PlayerAnimation::Spawn, spawn_len, { LoadTexture("player_spawn_1.png", size), LoadTexture("player_spawn_2.png", size), LoadTexture("player_spawn_3.png", size), LoadTexture("player_spawn_4.png", size) });
   player_1.SetFrames(PlayerAnimation::Destroy, death_len, { LoadTexture("player_1_death_1.png", size), LoadTexture("player_1_death_2.png", size), LoadTexture("player_1_death_3.png", size), LoadTexture("player_1_death_4.png", size) });
   mPlayerRes.insert({ player_1.GetType(), player_1 });

   PlayerResource player_2(PlayerType::Player_2);
   player_2.SetFrames(PlayerAnimation::StandUp, walk_len, { LoadTexture("player_2_up.png", size) });
   player_2.SetFrames(PlayerAnimation::StandDown, walk_len, { LoadTexture("player_2_down.png", size) });
   player_2.SetFrames(PlayerAnimation::StandLeft, walk_len, { LoadTexture("player_2_left.png", size) });
   player_2.SetFrames(PlayerAnimation::StandRight, walk_len, { LoadTexture("player_2_right.png", size) });
   player_2.SetFrames(PlayerAnimation::WalkUp, walk_len, { LoadTexture("player_2_up_1.png", size), LoadTexture("player_2_up_2.png", size) });
   player_2.SetFrames(PlayerAnimation::WalkDown, walk_len, { LoadTexture("player_2_down_1.png", size), LoadTexture("player_2_down_2.png", size) });
   player_2.SetFrames(PlayerAnimation::WalkLeft, walk_len, { LoadTexture("player_2_left_1.png", size), LoadTexture("player_2_left_2.png", size) });
   player_2.SetFrames(PlayerAnimation::WalkRight, walk_len, { LoadTexture("player_2_right_1.png", size), LoadTexture("player_2_right_2.png", size) });
   player_2.SetFrames(PlayerAnimation::Spawn, spawn_len, { LoadTexture("player_spawn_1.png", size), LoadTexture("player_spawn_2.png", size), LoadTexture("player_spawn_3.png", size), LoadTexture("player_spawn_4.png", size) });
   player_2.SetFrames(PlayerAnimation::Destroy, death_len, { LoadTexture("player_2_death_1.png", size), LoadTexture("player_2_death_2.png", size), LoadTexture("player_2_death_3.png", size), LoadTexture("player_2_death_4.png", size) });
   mPlayerRes.insert({ player_2.GetType(), player_2 });

   PlayerResource player_3(PlayerType::Player_3);
   player_3.SetFrames(PlayerAnimation::StandUp, walk_len, { LoadTexture("player_3_up.png", size) });
   player_3.SetFrames(PlayerAnimation::StandDown, walk_len, { LoadTexture("player_3_down.png", size) });
   player_3.SetFrames(PlayerAnimation::StandLeft, walk_len, { LoadTexture("player_3_left.png", size) });
   player_3.SetFrames(PlayerAnimation::StandRight, walk_len, { LoadTexture("player_3_right.png", size) });
   player_3.SetFrames(PlayerAnimation::WalkUp, walk_len, { LoadTexture("player_3_up_1.png", size), LoadTexture("player_3_up_2.png", size) });
   player_3.SetFrames(PlayerAnimation::WalkDown, walk_len, { LoadTexture("player_3_down_1.png", size), LoadTexture("player_3_down_2.png", size) });
   player_3.SetFrames(PlayerAnimation::WalkLeft, walk_len, { LoadTexture("player_3_left_1.png", size), LoadTexture("player_3_left_2.png", size) });
   player_3.SetFrames(PlayerAnimation::WalkRight, walk_len, { LoadTexture("player_3_right_1.png", size), LoadTexture("player_3_right_2.png", size) });
   player_3.SetFrames(PlayerAnimation::Spawn, spawn_len, { LoadTexture("player_spawn_1.png", size), LoadTexture("player_spawn_2.png", size), LoadTexture("player_spawn_3.png", size), LoadTexture("player_spawn_4.png", size) });
   player_3.SetFrames(PlayerAnimation::Destroy, death_len, { LoadTexture("player_3_death_1.png", size), LoadTexture("player_3_death_2.png", size), LoadTexture("player_3_death_3.png", size), LoadTexture("player_3_death_4.png", size) });
   mPlayerRes.insert({ player_3.GetType(), player_3 });

   PlayerResource player_4(PlayerType::Player_4);
   player_4.SetFrames(PlayerAnimation::StandUp, walk_len, { LoadTexture("player_4_up.png", size) });
   player_4.SetFrames(PlayerAnimation::StandDown, walk_len, { LoadTexture("player_4_down.png", size) });
   player_4.SetFrames(PlayerAnimation::StandLeft, walk_len, { LoadTexture("player_4_left.png", size) });
   player_4.SetFrames(PlayerAnimation::StandRight, walk_len, { LoadTexture("player_4_right.png", size) });
   player_4.SetFrames(PlayerAnimation::WalkUp, walk_len, { LoadTexture("player_4_up_1.png", size), LoadTexture("player_4_up_2.png", size) });
   player_4.SetFrames(PlayerAnimation::WalkDown, walk_len, { LoadTexture("player_4_down_1.png", size), LoadTexture("player_4_down_2.png", size) });
   player_4.SetFrames(PlayerAnimation::WalkLeft, walk_len, { LoadTexture("player_4_left_1.png", size), LoadTexture("player_4_left_2.png", size) });
   player_4.SetFrames(PlayerAnimation::WalkRight, walk_len, { LoadTexture("player_4_right_1.png", size), LoadTexture("player_4_right_2.png", size) });
   player_4.SetFrames(PlayerAnimation::Spawn, spawn_len, { LoadTexture("player_spawn_1.png", size), LoadTexture("player_spawn_2.png", size), LoadTexture("player_spawn_3.png", size), LoadTexture("player_spawn_4.png", size) });
   player_4.SetFrames(PlayerAnimation::Destroy, death_len, { LoadTexture("player_4_death_1.png", size), LoadTexture("player_4_death_2.png", size), LoadTexture("player_4_death_3.png", size), LoadTexture("player_4_death_4.png", size) });
   mPlayerRes.insert({ player_4.GetType(), player_4 });
}

SDL_Surface* ResourceCache::LoadTexture(const std::string& file, const Size& size)
{
   const auto full_path = mResDir + "/sprite/" + file;

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
