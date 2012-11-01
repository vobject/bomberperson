#ifndef SDL_RESOURCE_CACHE_HPP
#define SDL_RESOURCE_CACHE_HPP

#include "SpriteResource.hpp"
#include "ArenaResource.hpp"
#include "WallResource.hpp"
#include "ExtraResource.hpp"
#include "BombResource.hpp"
#include "ExplosionResource.hpp"
#include "PlayerResource.hpp"
#include "../../utils/Utils.hpp"

#include <map>
#include <string>
#include <vector>

class BomberPersonConfig;
enum class EntityId;
struct SDL_Surface;
class TiXmlHandle;

// TODO: Put this in its own namespace.

class ResourceCache
{
public:
   ResourceCache(const BomberPersonConfig& app_cfg);
   ~ResourceCache();

   ResourceCache(const ResourceCache&) = delete;
   ResourceCache& operator=(const ResourceCache&) = delete;

   // TODO:
   // SetTheme();

   SpriteResource GetMenuResource(EntityId id) const;
   ArenaResource GetArenaResource(ArenaType type) const;
   WallResource GetWallResource(WallType type) const;
   ExtraResource GetExtraResource(ExtraType type) const;
   BombResource GetBombResource(BombType type) const;
   ExplosionResource GetExplosionResource(ExplosionType type) const;
   PlayerResource GetPlayerResource(PlayerType type) const;

private:
   void LoadMenuResources();
   void LoadArenaResources(const TiXmlHandle& hndl);
   void LoadWallResources(const TiXmlHandle& hndl);
   void LoadExtraResources(const TiXmlHandle& hndl);
   void LoadBombResources(const TiXmlHandle& hndl);
   void LoadExplosionResources(const TiXmlHandle& hndl);
   void LoadPlayerResources(const TiXmlHandle& hndl);

   PlayerResource LoadPlayer(PlayerType type, const std::string& name, const TiXmlHandle& hndl);
   std::vector<SDL_Surface*> LoadTextures(const TiXmlHandle& hndl, const std::string& name, Size size);
   SDL_Surface* LoadTexture(const std::string& file, const Size& size);

   const BomberPersonConfig& mAppConfig;
   const std::string mResourceDir;
   const Size mResolution;
   std::map<EntityId, SpriteResource> mMenuRes;
   std::map<ArenaType, ArenaResource> mArenaRes;
   std::map<WallType, WallResource> mWallRes;
   std::map<ExtraType, ExtraResource> mExtraRes;
   std::map<BombType, BombResource> mBombRes;
   std::map<ExplosionType, ExplosionResource> mExplosionRes;
   std::map<PlayerType, PlayerResource> mPlayerRes;
   std::vector<SDL_Surface*> mSurfaceCache;
};

#endif // SDL_RESOURCE_CACHE_HPP
