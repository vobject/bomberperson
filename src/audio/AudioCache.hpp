#ifndef AUDIO_CACHE_HPP
#define AUDIO_CACHE_HPP

#include <map>
#include <string>

#include <SDL_mixer.h>

enum class MenuMusic;
enum class ArenaMusic;

enum class MenuSound;
enum class BombSound;
enum class ExplosionSound;
enum class PlayerSound;

class AudioCache
{
public:
   AudioCache(const std::string& res_dir);
   ~AudioCache();

   AudioCache(const AudioCache&) = delete;
   AudioCache& operator=(const AudioCache&) = delete;

   Mix_Music* GetMusic(MenuMusic type) const;
   Mix_Music* GetMusic(ArenaMusic type) const;

   Mix_Chunk* GetSound(MenuSound type) const;
   Mix_Chunk* GetSound(BombSound type) const;
   Mix_Chunk* GetSound(ExplosionSound type) const;
   Mix_Chunk* GetSound(PlayerSound type) const;

private:
   void LoadMenuResources();
   void LoadArenaResources();
   void LoadBombResources();
   void LoadExplosionResources();
   void LoadPlayerResources();

   Mix_Music* LoadMusic(const std::string& file);
   Mix_Chunk* LoadSound(const std::string& file);

   const std::string mResourceDir;
   std::map<MenuMusic, Mix_Music*> mMenuMusic;
   std::map<MenuSound, Mix_Chunk*> mMenuSound;
   std::map<ArenaMusic, Mix_Music*> mArenaMusic;
   std::map<BombSound, Mix_Chunk*> mBombSound;
   std::map<ExplosionSound, Mix_Chunk*> mExplosionSound;
   std::map<PlayerSound, Mix_Chunk*> mPlayerSound;
};

#endif // AUDIO_CACHE_HPP
