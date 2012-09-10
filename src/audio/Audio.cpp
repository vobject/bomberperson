#include "Audio.hpp"
#include "../game/Match.hpp"
#include "../game/Arena.hpp"
#include "../game/Scoreboard.hpp"
#include "../game/Cell.hpp"
#include "../game/Wall.hpp"
#include "../game/Extra.hpp"
#include "../game/Bomb.hpp"
#include "../game/Explosion.hpp"
#include "../game/Player.hpp"

#include <SDL.h>
#include <SDL_mixer.h>

//The music that will be played
Mix_Music *music = NULL;

Mix_Chunk *explosion = NULL;
Mix_Chunk *plant = NULL;
Mix_Chunk *extra = NULL;
Mix_Chunk *death = NULL;

Audio::Audio()
{

   // SOUND TEST

   if(0 > SDL_Init(SDL_INIT_AUDIO)) {
      throw "Cannot init SDL audio subsystem.";
   }
   atexit(SDL_Quit);

   if(0 > Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096)) {
      throw "Cannot init SDL audio subsystem.";
   }

//   //The sound effects that will be used
//   Mix_Chunk *scratch = NULL;
//   Mix_Chunk *high = NULL;
//   Mix_Chunk *med = NULL;
//   Mix_Chunk *low = NULL;

//   //Load the music
//   music = Mix_LoadMUS( "sound4.mp3" );

//   //If there was a problem loading the music
//   if( music == NULL ) {
//      throw "Music load failed.";
//   }

   //Load the sound effects
   explosion = Mix_LoadWAV( "explosion.wav" );
   death = Mix_LoadWAV( "d.wav" );
   extra = Mix_LoadWAV( "extra.wav" );
   plant = Mix_LoadWAV( "plant.wav" );

   //If there was a problem loading the sound effects
   if( ( explosion == NULL ) || ( death == NULL ) || (extra == NULL) || (plant == NULL))
   {
      throw "Sound load failed.";
   }

//   //If there is no music playing
//   if( Mix_PlayingMusic() == 0 )
//   {
//      //Play the music
//      if( Mix_PlayMusic( music, -1 ) == -1 ) {
//         throw "Music play failed.";
//      }
//   }
}

Audio::~Audio()
{
   //Free the music
//   Mix_FreeMusic( music );

   Mix_FreeChunk( death );
   Mix_FreeChunk( extra );
   Mix_FreeChunk( explosion );
   Mix_FreeChunk( plant );

   //Quit SDL_mixer
   Mix_CloseAudio();
}

void Audio::Play(const std::shared_ptr<Match> &match)
{
//   for (const auto& cell : match->GetArena()->GetCells())
//   {
//      if (cell->HasBomb() && (SoundId::None != cell->GetBomb()->GetSound()))
//      {
//         if( Mix_PlayChannel( -1, plant, 0 ) == -1 ) {
//            throw "Sound play failed.";
//         }
//      }

//      if (cell->HasExplosion() && (SoundId::None != cell->GetExplosion()->GetSound()))
//      {
//         if( Mix_PlayChannel( -1, explosion, 0 ) == -1 ) {
//            throw "Sound play failed.";
//         }
//      }
//   }

//   for (const auto& player : match->GetPlayers())
//   {
//      const auto s = player->GetSound();
//      if (SoundId::None == s) {
//         continue;
//      }

//      if (SoundId::PlayerPicksUpExtra == s)
//      {
//         if( Mix_PlayChannel( -1, extra, 0 ) == -1 ) {
//            throw "Sound play failed.";
//         }
//      }

//      if (SoundId::PlayerDies == s)
//      {
//         if( Mix_PlayChannel( -1, death, 0 ) == -1 ) {
//            throw "Sound play failed.";
//         }
//      }
//   }
}

void Audio::PlayMusic(MusicId id)
{

}

void Audio::PlaySound(SoundId id)
{

}

//void WindowFrame::UpdateDone()
//{
//   mUpdateCount++;
//}
//
//void WindowFrame::FrameDone()
//{
//   mFrameCount++;
//}
