#ifndef AUDIO_HPP
#define AUDIO_HPP

#include <memory>

class SceneObject;
class MainMenu;
class Arena;
class Scoreboard;
class Wall;
class Extra;
class Bomb;
class Explosion;
class Player;
class AudioCache;

//enum class MusicId
//{
//   MainMenu,
////   GameSetup,
//   Running
//};

//class SampleResource
//{

//};

//class MusicResource
//{

//};

//class AudioCache
//{

//};

class Audio
{
public:
   Audio();
   ~Audio();

   Audio(const Audio&) = delete;
   Audio& operator=(const Audio&) = delete;

   void Play(const std::shared_ptr<SceneObject>& ent);

   void Play(const std::shared_ptr<MainMenu>& mainmenu);
   void Play(const std::shared_ptr<Arena>& arena);
   void Play(const std::shared_ptr<Scoreboard>& scoreboard);
   void Play(const std::shared_ptr<Wall>& explosion);
   void Play(const std::shared_ptr<Extra>& bomb);
   void Play(const std::shared_ptr<Bomb>& bomb);
   void Play(const std::shared_ptr<Explosion>& explosion);
   void Play(const std::shared_ptr<Player>& player);

   // TODO: louder/softer

private:
   bool mPlayingMenuMusic = false;
   bool mPlayingGameMusic = false;

   std::unique_ptr<AudioCache> mCache;
};

#endif // AUDIO_HPP
