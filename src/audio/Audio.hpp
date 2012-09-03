#ifndef AUDIO_HPP
#define AUDIO_HPP

//#include "utils/Utils.hpp"
//#include "Options.hpp"
//
//#include <SDL.h>
//
//#include <string>
#include <memory>

class Match;
enum class SoundId;

enum class MusicId
{
   MainMenu,
//   GameSetup,
   Running
};

class SampleResource
{

};

class MusicResource
{

};

class AudioCache
{

};

class Audio
{
public:
   Audio();
   ~Audio();

   void Play(const std::shared_ptr<Match>& match);

   // louder/softer
   void PlayMusic(MusicId id);
   void PlaySound(SoundId id);

private:
   std::unique_ptr<AudioCache> mCache = nullptr;
};

#endif // AUDIO_HPP
