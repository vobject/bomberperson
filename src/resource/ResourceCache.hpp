#ifndef RESOURCE_CACHE_HPP
#define RESOURCE_CACHE_HPP

#include "SpriteResource.hpp"
#include "WallResource.hpp"
#include "ExtraResource.hpp"
#include "PlayerResource.hpp"
#include "../utils/Utils.hpp"

#include <map>
#include <string>
#include <vector>

struct SDL_Surface;
enum class EntityId;
enum class PlayerType;

class ResourceCache
{
public:
   ResourceCache();
   ~ResourceCache();

   ResourceCache(const ResourceCache&) = delete;
   ResourceCache& operator=(const ResourceCache&) = delete;

   // TODO:
   // SetTheme();

   SpriteResource GetMenuResource(EntityId id) const;
   SpriteResource GetArenaResource(EntityId id) const;
   WallResource GetWallResource(WallType type) const;
   ExtraResource GetExtraResource(ExtraType type) const;
   SpriteResource GetBombResource(EntityId id) const;
   SpriteResource GetExplosionResource(EntityId id) const;
   PlayerResource GetPlayerResource(PlayerType type) const;

//   Texture GetPlayer(const Kinect& kinect);
   // std::shared_ptr<...> GetAudioSample(const std::string& id);

//   void AddDirectory(const std::string& dir);
//   SDL_Surface* GetResource(const std::string& id);

private:
   void LoadMenuResources();
   void LoadArenaResources();
   void LoadWallResources();
   void LoadExtraResources();
   void LoadBombResources();
   void LoadExplosionResources();
   void LoadPlayerResources();

   SDL_Surface* LoadTexture(const std::string& file, const Size& size);

   std::string mResDir;
   std::map<EntityId, SpriteResource> mMenuRes;
   std::map<EntityId, SpriteResource> mArenaRes;
   std::map<WallType, WallResource> mWallRes;
   std::map<ExtraType, ExtraResource> mExtraRes;
   std::map<EntityId, SpriteResource> mBombRes;
   std::map<EntityId, SpriteResource> mExplosionRes;
   std::map<PlayerType, PlayerResource> mPlayerRes;
   std::vector<SDL_Surface*> mSurfaceCache;
};

#endif // RESOURCE_CACHE_HPP
