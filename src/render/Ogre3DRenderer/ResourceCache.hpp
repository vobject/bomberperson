#ifndef OGRE3D_RESOURCE_CACHE_HPP
#define OGRE3D_RESOURCE_CACHE_HPP

// #include "SpriteResource.hpp"
// #include "WallResource.hpp"
// #include "ExtraResource.hpp"
// #include "BombResource.hpp"
// #include "ExplosionResource.hpp"
// #include "PlayerResource.hpp"
// #include "../../utils/Utils.hpp"

#include <memory>
#include <map>
#include <string>
#include <vector>

namespace Ogre {
   class Root;
}

// enum class EntityId;

class Ogre3DResourceCache
{
public:
   Ogre3DResourceCache(const std::string& renderer_dir);
   ~Ogre3DResourceCache();

   Ogre3DResourceCache(const Ogre3DResourceCache&) = delete;
   Ogre3DResourceCache& operator=(const Ogre3DResourceCache&) = delete;

//    SpriteResource GetMenuResource(EntityId id) const;
//    SpriteResource GetArenaResource(EntityId id) const;
//    WallResource GetWallResource(WallType type) const;
//    ExtraResource GetExtraResource(ExtraType type) const;
//    BombResource GetBombResource(BombType type) const;
//    ExplosionResource GetExplosionResource(ExplosionType type) const;
//    PlayerResource GetPlayerResource(PlayerType type) const;

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

    std::string mResDir;
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
