#ifndef RESOURCE_CACHE_HPP
#define RESOURCE_CACHE_HPP

#include "SpriteResource.hpp"
#include "PlayerResource.hpp"
#include "../utils/Utils.hpp"

#include <string>
#include <vector>
#include <map>

struct SDL_Surface;

class ResourceCache
{
public:
   ResourceCache();
   ~ResourceCache();

   ResourceCache(const ResourceCache&) = delete;
   ResourceCache& operator=(const ResourceCache&) = delete;

   // TODO:
   // SetTheme();

   SpriteResource GetBgResource(const std::string& id) const;
   SpriteResource GetWallResource(const std::string& id) const;
   SpriteResource GetExtraResource(const std::string& id) const;
   SpriteResource GetBombResource(const std::string& id) const;
   SpriteResource GetExplosionResource(const std::string& id) const;
   PlayerResource GetPlayerResource(const std::string& id) const;

//   Texture GetPlayer(const Kinect& kinect);
   // std::shared_ptr<...> GetAudioSample(const std::string& id);

//   void AddDirectory(const std::string& dir);
//   SDL_Surface* GetResource(const std::string& id);

private:
   void LoadBackgroundResources();
   void LoadWallResources();
   void LoadExtraResources();
   void LoadBombResources();
   void LoadExplosionResources();
   void LoadPlayerResources();

   SDL_Surface* LoadTexture(const std::string& file, const Size& size);

   std::string mResDir;
   std::map<std::string, SpriteResource> mBackgroundRes;
   std::map<std::string, SpriteResource> mWallRes;
   std::map<std::string, SpriteResource> mExtraRes;
   std::map<std::string, SpriteResource> mBombRes;
   std::map<std::string, SpriteResource> mExplosionRes;
   std::map<std::string, PlayerResource> mPlayerRes;
   std::vector<SDL_Surface*> mSurfaceCache;
};

#endif // RESOURCE_CACHE_HPP
