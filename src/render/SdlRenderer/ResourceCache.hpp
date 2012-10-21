#ifndef SDL_RESOURCE_CACHE_HPP
#define SDL_RESOURCE_CACHE_HPP

#include "SpriteResource.hpp"
#include "WallResource.hpp"
#include "ExtraResource.hpp"
#include "BombResource.hpp"
#include "ExplosionResource.hpp"
#include "PlayerResource.hpp"
#include "../../utils/Utils.hpp"

#include <map>
#include <string>
#include <vector>

struct SDL_Surface;
class TiXmlHandle;
enum class EntityId;

// TODO: Put this in its own namespace.

class ResourceCache
{
public:
   ResourceCache(const std::string& renderer_dir);
   ~ResourceCache();

   ResourceCache(const ResourceCache&) = delete;
   ResourceCache& operator=(const ResourceCache&) = delete;

   // TODO:
   // SetTheme();

   SpriteResource GetMenuResource(EntityId id) const;
   SpriteResource GetArenaResource(EntityId id) const;
   WallResource GetWallResource(WallType type) const;
   ExtraResource GetExtraResource(ExtraType type) const;
   BombResource GetBombResource(BombType type) const;
   ExplosionResource GetExplosionResource(ExplosionType type) const;
   PlayerResource GetPlayerResource(PlayerType type) const;

//   Texture GetPlayer(const Kinect& kinect);
   // std::shared_ptr<...> GetAudioSample(const std::string& id);

private:
   void LoadMenuResources();
   void LoadArenaResources();
   void LoadWallResources(const TiXmlHandle& hndl);
   void LoadExtraResources(const TiXmlHandle& hndl);
   void LoadBombResources();
   void LoadExplosionResources();
   void LoadPlayerResources();

   SDL_Surface* LoadTexture(const std::string& file, const Size& size);

   const std::string mResDir;
   std::map<EntityId, SpriteResource> mMenuRes;
   std::map<EntityId, SpriteResource> mArenaRes;
   std::map<WallType, WallResource> mWallRes;
   std::map<ExtraType, ExtraResource> mExtraRes;
   std::map<BombType, BombResource> mBombRes;
   std::map<ExplosionType, ExplosionResource> mExplosionRes;
   std::map<PlayerType, PlayerResource> mPlayerRes;
   std::vector<SDL_Surface*> mSurfaceCache;
};

#endif // SDL_RESOURCE_CACHE_HPP
