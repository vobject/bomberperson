#include "AudioCache.hpp"
#include "../game/MainMenu.hpp"
#include "../game/Arena.hpp"
#include "../game/Bomb.hpp"
#include "../game/Explosion.hpp"
#include "../game/Player.hpp"
#include "../Options.hpp"

AudioCache::AudioCache()
   : mResDir(RESOURCE_DIR)
{
   // TODO: Coordinate this using XML files.
   LoadMenuResources();
   LoadArenaResources();
   LoadBombResources();
   LoadExplosionResources();
   LoadPlayerResources();
}

AudioCache::~AudioCache()
{
   for (const auto& music : mMenuMusic) {
      Mix_FreeMusic(music.second);
   }

   for (const auto& music : mArenaMusic) {
      Mix_FreeMusic(music.second);
   }

   for (const auto& sound : mMenuSound) {
      Mix_FreeChunk(sound.second);
   }

   for (const auto& sound : mBombSound) {
      Mix_FreeChunk(sound.second);
   }

   for (const auto& sound : mExplosionSound) {
      Mix_FreeChunk(sound.second);
   }

   for (const auto& sound : mPlayerSound) {
      Mix_FreeChunk(sound.second);
   }
}

Mix_Music* AudioCache::GetMusic(const MenuMusic type) const
{
   const auto iter = mMenuMusic.find(type);
   if (iter == mMenuMusic.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

Mix_Music* AudioCache::GetMusic(const ArenaMusic type) const
{
   const auto iter = mArenaMusic.find(type);
   if (iter == mArenaMusic.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

Mix_Chunk* AudioCache::GetSound(const MenuSound type) const
{
   const auto iter = mMenuSound.find(type);
   if (iter == mMenuSound.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

Mix_Chunk* AudioCache::GetSound(const BombSound type) const
{
   const auto iter = mBombSound.find(type);
   if (iter == mBombSound.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

Mix_Chunk* AudioCache::GetSound(const ExplosionSound type) const
{
   const auto iter = mExplosionSound.find(type);
   if (iter == mExplosionSound.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

Mix_Chunk* AudioCache::GetSound(const PlayerSound type) const
{
   const auto iter = mPlayerSound.find(type);
   if (iter == mPlayerSound.end()) {
      throw "Trying to access non-existing resource";
   }
   return iter->second;
}

void AudioCache::LoadMenuResources()
{
   mMenuMusic.insert({ MenuMusic::MainmenuTrack, LoadMusic("menu_main.mp3") });

   mMenuSound.insert({ MenuSound::Switch, LoadSound("menu_switch.wav") });
   mMenuSound.insert({ MenuSound::Choose, LoadSound("menu_choose.wav") });
   mMenuSound.insert({ MenuSound::Invalid, LoadSound("menu_invalid.wav") });
}

void AudioCache::LoadArenaResources()
{
   mArenaMusic.insert({ ArenaMusic::DefaultTrack, LoadMusic("arena_default.mp3") });
}

void AudioCache::LoadBombResources()
{
   mBombSound.insert({ BombSound::Planted, LoadSound("bomb_planted.wav") });
}

void AudioCache::LoadExplosionResources()
{
   mExplosionSound.insert({ ExplosionSound::Booom, LoadSound("explosion_booom.wav") });
}

void AudioCache::LoadPlayerResources()
{
   mPlayerSound.insert({ PlayerSound::Collect_Speed, LoadSound("player_collect_speed.wav") });
   mPlayerSound.insert({ PlayerSound::Collect_Bombs, LoadSound("player_collect_bombs.wav") });
   mPlayerSound.insert({ PlayerSound::Collect_Range, LoadSound("player_collect_range.wav") });
   mPlayerSound.insert({ PlayerSound::Collect_InfiniteRange, LoadSound("player_collect_infiniterange.wav") });
   mPlayerSound.insert({ PlayerSound::Die, LoadSound("player_die.wav") });
//   mPlayerSound.insert({ PlayerSound::Win, LoadSound("player_.wav") });
}

Mix_Music *AudioCache::LoadMusic(const std::string& file)
{
   const auto full_path = mResDir + "/music/" + file;

   auto music = Mix_LoadMUS(full_path.c_str());
   if (!music) {
      throw "Failed to load music file.";
   }
   return music;
}

Mix_Chunk *AudioCache::LoadSound(const std::string& file)
{
   const auto full_path = mResDir + "/sound/" + file;

   auto sound = Mix_LoadWAV(full_path.c_str());
   if (!sound) {
      throw "Failed to load music file.";
   }
   return sound;
}
