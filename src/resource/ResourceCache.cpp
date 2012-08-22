#include "ResourceCache.hpp"
#include "../utils/Utils.hpp"
#include "../Options.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_rotozoom.h>

//#include <boost/filesystem.hpp>
//namespace boostfs = boost::filesystem;

ResourceCache::ResourceCache()
   : mResDir(DefaultOptions::RESOURCE_DIR)
{
   if (0 == IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)) {
       throw "Failed to initialize SDL_image";
   }

   // TODO: Coordinate this using XML files.
   LoadBackgroundResources();
//   LoadArenaResources();
   LoadCellResources();
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

//void ResourceCache::AddDirectory(const std::string& dir)
//{
//   boostfs::path path(dir);
//
//   try
//   {
//      if (!boostfs::exists(path) || !boostfs::is_directory(path)) {
//         LOG(logDEBUG) << "Resource directory does not exist: " << path;
//         return;
//      }
//
//      boostfs::directory_iterator dir_iter(path);
//      boostfs::directory_iterator end_iter;
//
//      for (; dir_iter != end_iter; dir_iter++)
//      {
//         if (boostfs::is_directory(*dir_iter)) {
//            continue;
//         }
//
//         Load(dir_iter->path().c_str(), dir_iter->path().filename().c_str());
//      }
//   }
//   catch (const boostfs::filesystem_error& ex)
//   {
//      LOG(logERROR) << "File system exception while adding resource directory.";
//   }
//}


//SpriteResource ResourceCache::GetSprite(const std::string& id)
//{
//   return mSprites[id];
//}

SpriteResource ResourceCache::GetBgResource(const std::string& id) const
{
   const auto iter = mBackgroundRes.find(id);
   if (iter == mBackgroundRes.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

//SpriteResource ResourceCache::GetArenaResource(const std::string& id) const
//{
//   const auto iter = mArenaRes.find(id);
//   if (iter == mArenaRes.end()) {
//      throw "Trying to access non-existing resource";
//   }
//   return iter->second;
//}

SpriteResource ResourceCache::GetCellResource(const std::string& id) const
{
   const auto iter = mCellRes.find(id);
   if (iter == mCellRes.end()) {
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

//void ResourceCache::LoadSprite(
//   const std::string& id,
//   const std::vector<std::string>& files,
//   const Size& size
//)
//{
//   std::vector<SDL_Surface*> textures;
//
//   for (const auto& file : files)
//   {
//      textures.push_back(LoadTexture(file, size));
//   }
//
//   mSprites[id] = { id, textures };
//}


void ResourceCache::LoadBackgroundResources()
{
   const auto bg_arena_1_id = DefaultOptions::ARENA_BG_ID;
   const Size size = { DefaultOptions::ARENA_BG_WIDTH,
                       DefaultOptions::ARENA_BG_HEIGHT };

   mBackgroundRes[bg_arena_1_id] = { bg_arena_1_id, { LoadTexture("../res_nonfree/bg/arena_2.png", size) } };
}

//void ResourceCache::LoadArenaResources()
//{
//   const auto arena_id = DefaultOptions::ARENA_ID;
//   const Size size = { DefaultOptions::ARENA_WIDTH,
//                       DefaultOptions::ARENA_HEIGHT };
//
//   mArenaRes[arena_id] = { arena_id, { LoadTexture("../res_nonfree/sprite/arena_1.png", size) } };
//}

void ResourceCache::LoadCellResources()
{
   const auto cell_id = DefaultOptions::CELL_ID;
   const Size size = { DefaultOptions::CELL_WIDTH,
                       DefaultOptions::CELL_HEIGHT };

   mCellRes[cell_id] = { cell_id, { LoadTexture("sprite/cell.png", size) } };
}

void ResourceCache::LoadWallResources()
{
   const auto wood_id = DefaultOptions::WALL_ID_WOOD;
   const auto bricks_id = DefaultOptions::WALL_ID_BRICKS;
   const Size size = { DefaultOptions::WALL_WIDTH,
                       DefaultOptions::WALL_HEIGHT };

   mWallRes[wood_id] = { wood_id, { LoadTexture("../res_nonfree/sprite/wall_wood.png", size) } };
   mWallRes[bricks_id] = { bricks_id, { LoadTexture("sprite/wall_bricks.png", size) } };
}

void ResourceCache::LoadExtraResources()
{
   const auto speed_id = DefaultOptions::EXTRA_ID_SPEED;
   const auto bombs_id = DefaultOptions::EXTRA_ID_BOMBS;
   const auto range_id = DefaultOptions::EXTRA_ID_RANGE;
   const Size size = { DefaultOptions::EXTRA_WIDTH,
                       DefaultOptions::EXTRA_HEIGHT };

   mExtraRes[speed_id] = { speed_id, { LoadTexture("sprite/extra_speed.png", size) } };
   mExtraRes[bombs_id] = { bombs_id, { LoadTexture("sprite/extra_supply.png", size) } };
   mExtraRes[range_id] = { range_id, { LoadTexture("sprite/extra_range.png", size) } };
}

void ResourceCache::LoadBombResources()
{
   const auto bomb_id = DefaultOptions::BOMB_ID;
   const Size size = { DefaultOptions::BOMB_WIDTH,
                       DefaultOptions::BOMB_HEIGHT };

   mBombRes[bomb_id] = {
      bomb_id,
      { LoadTexture("sprite/bomb_1.png", size),
        LoadTexture("sprite/bomb_2.png", size) }
   };
}

void ResourceCache::LoadExplosionResources()
{
   const auto explosion_id = DefaultOptions::EXPLOSION_ID;
   const Size size = { DefaultOptions::EXPLOSION_WIDTH,
                       DefaultOptions::EXPLOSION_HEIGHT };

   mExplosionRes[explosion_id] = {
      explosion_id,
      { LoadTexture("sprite/explosion_1.png", size),
        LoadTexture("sprite/explosion_2.png", size),
        LoadTexture("sprite/explosion_3.png", size),
        LoadTexture("sprite/explosion_4.png", size) }
   };
}

void ResourceCache::LoadPlayerResources()
{
   const Size size = { DefaultOptions::PLAYER_WIDTH,
                       DefaultOptions::PLAYER_HEIGHT };

   PlayerResource player_1(DefaultOptions::PLAYER_ID_1);
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

   PlayerResource player_2(DefaultOptions::PLAYER_ID_2);
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
