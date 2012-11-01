#ifndef OGRE3D_RESOURCE_CACHE_HPP
#define OGRE3D_RESOURCE_CACHE_HPP

// #include "SpriteResource.hpp"
// #include "WallResource.hpp"
// #include "ExtraResource.hpp"
// #include "BombResource.hpp"
// #include "ExplosionResource.hpp"
// #include "PlayerResource.hpp"

#include "../../utils/Utils.hpp"

#include <memory>
#include <map>
#include <string>
#include <vector>

namespace Ogre {
   class Root;
}

enum class WallType;
enum class ExtraType;
enum class BombType;
enum class ExplosionType;
enum class PlayerType;

class Ogre3DResourceCache
{
public:
   Ogre3DResourceCache(const std::string& renderer_dir, Size res);
   ~Ogre3DResourceCache();

   Ogre3DResourceCache(const Ogre3DResourceCache&) = delete;
   Ogre3DResourceCache& operator=(const Ogre3DResourceCache&) = delete;

   void LoadResources();

   std::string GetArenaResource() const;
   std::string GetWallResource(WallType type) const;
   std::string GetExtraResource(ExtraType type) const;
   std::string GetBombResource(BombType type) const;
   std::string GetExplosionResource(ExplosionType type) const;
   std::string GetPlayerResource(PlayerType type) const;

private:
   void InitRoot();

//    void LoadMenuResources();
//    void LoadArenaResources();
//    void LoadWallResources();
//    void LoadExtraResources();
//    void LoadBombResources();
//    void LoadExplosionResources();
//    void LoadPlayerResources();
// 
//    SDL_Surface* LoadTexture(const std::string& file, const Size& size);

    const std::string mResourceDir;
    const Size mResolution;
    std::unique_ptr<Ogre::Root> mRoot;

//    std::map<EntityId, SpriteResource> mMenuRes;
//    std::map<EntityId, SpriteResource> mArenaRes;
//    std::map<WallType, WallResource> mWallRes;
//    std::map<ExtraType, ExtraResource> mExtraRes;
//    std::map<BombType, BombResource> mBombRes;
//    std::map<ExplosionType, ExplosionResource> mExplosionRes;
//    std::map<PlayerType, PlayerResource> mPlayerRes;
//    std::vector<SDL_Surface*> mSurfaceCache;
};

#endif // OGRE3D_RESOURCE_CACHE_HPP
