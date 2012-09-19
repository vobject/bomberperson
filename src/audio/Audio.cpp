#include "Audio.hpp"
#include "../game/Match.hpp"
#include "../game/MainMenu.hpp"
#include "../game/Arena.hpp"
#include "../game/Scoreboard.hpp"
#include "../game/Wall.hpp"
#include "../game/Extra.hpp"
#include "../game/Bomb.hpp"
#include "../game/Explosion.hpp"
#include "../game/Player.hpp"
#include "AudioCache.hpp"

#include <SDL.h>
#include <SDL_mixer.h>

Audio::Audio()
{
   if(0 > SDL_Init(SDL_INIT_AUDIO)) {
      throw "Cannot init SDL audio subsystem.";
   }
   atexit(SDL_Quit);

   if(0 > Mix_OpenAudio(MIX_DEFAULT_FREQUENCY,
                        MIX_DEFAULT_FORMAT,
                        MIX_DEFAULT_CHANNELS,
                        4096))
   {
      throw "Cannot init SDL audio subsystem.";
   }

   Mix_Volume(-1, MIX_MAX_VOLUME / 6);
   Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

   mCache = make_unique<AudioCache>();
}

Audio::~Audio()
{
   Mix_CloseAudio();
}

void Audio::Play(const std::shared_ptr<SceneObject>& ent)
{
   if (const auto ptr = std::dynamic_pointer_cast<MainMenu>(ent)) {
      Play(ptr);
   }
   else if (const auto ptr = std::dynamic_pointer_cast<Arena>(ent)) {
      Play(ptr);
   }
   else if (const auto ptr = std::dynamic_pointer_cast<Scoreboard>(ent)) {
      Play(ptr);
   }
   else if (const auto ptr = std::dynamic_pointer_cast<Wall>(ent)) {
      Play(ptr);
   }
   else if (const auto ptr = std::dynamic_pointer_cast<Bomb>(ent)) {
      Play(ptr);
   }
   else if (const auto ptr = std::dynamic_pointer_cast<Explosion>(ent)) {
      Play(ptr);
   }
   else if (const auto ptr = std::dynamic_pointer_cast<Extra>(ent)) {
      Play(ptr);
   }
   else if (const auto ptr = std::dynamic_pointer_cast<Player>(ent)) {
      Play(ptr);
   }
   else {
//      LOG(logERROR) << "Audio::Play(SceneObject) Unknown object!";
   }
}

void Audio::Play(const std::shared_ptr<MainMenu>& mainmenu)
{
   // Take care of the menu music effects first.

   if (mPlayingGameMusic)
   {
      // We just switched from the game screen to the mainmenu.
      Mix_HaltMusic();
      mPlayingGameMusic = false;
   }

   if (!Mix_PlayingMusic())
   {
      // Play mainmenu music if it is not already running.
      if (Mix_PlayMusic(mCache->GetMusic(MenuMusic::MainmenuTrack), -1) == -1) {
         throw "Sound music failed.";
      }
      mPlayingMenuMusic = true;
   }

   // Now handle the menu sound effects.

   const auto sound = mainmenu->GetSound(true);
   if (MenuSound::None != sound)
   {
      if (0 > Mix_PlayChannel( -1, mCache->GetSound(sound), 0 )) {
         throw "Sound play failed.";
      }
   }
}

void Audio::Play(const std::shared_ptr<Arena>& arena)
{
   if (mPlayingMenuMusic)
   {
      Mix_HaltMusic();
      mPlayingMenuMusic = false;
   }

   if (Mix_PlayingMusic()) {
      return;
   }

   if (Mix_PlayMusic(mCache->GetMusic(ArenaMusic::DefaultTrack), -1) == -1) {
      throw "Sound music failed.";
   }
   mPlayingGameMusic = true;
}

void Audio::Play(const std::shared_ptr<Scoreboard>& scoreboard)
{
   (void) scoreboard;
}

void Audio::Play(const std::shared_ptr<Wall>& wall)
{
   (void) wall;
}

void Audio::Play(const std::shared_ptr<Extra>& extra)
{
   (void) extra;
}

void Audio::Play(const std::shared_ptr<Bomb>& bomb)
{
   const auto sound = bomb->GetSound(true);
   if (BombSound::None == sound) {
      return;
   }

   if (0 > Mix_PlayChannel( -1, mCache->GetSound(sound), 0 )) {
      throw "Sound play failed.";
   }
}

void Audio::Play(const std::shared_ptr<Explosion>& explosion)
{
   const auto sound = explosion->GetSound(true);
   if (ExplosionSound::None == sound) {
      return;
   }

   if (0 > Mix_PlayChannel( -1, mCache->GetSound(sound), 0 )) {
      throw "Sound play failed.";
   }
}

void Audio::Play(const std::shared_ptr<Player>& player)
{
   const auto sound = player->GetSound(true);
   if (PlayerSound::None == sound) {
      return;
   }

   if (0 > Mix_PlayChannel( -1, mCache->GetSound(sound), 0 )) {
      throw "Sound play failed.";
   }
}
