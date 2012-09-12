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

   // louder/softer
//   void PlayMusic(MusicId id);
//   void PlaySound(SoundId id);

private:
//   std::unique_ptr<AudioCache> mCache;
};

#endif // AUDIO_HPP
