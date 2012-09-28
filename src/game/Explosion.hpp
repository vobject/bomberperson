#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "ArenaObject.hpp"
#include "EventListener.hpp"

class EventQueue;
class SpawnExplosionStartEvent;
class SpawnExplosionEndEvent;
class DestroyExplosionStartEvent;
class DestroyExplosionEndEvent;

enum class PlayerType;

enum class ExplosionType
{
   Center,
   Horizontal,
   HorizontalLeftEnd,
   HorizontalRightEnd,
   Vertical,
   VerticalUpEnd,
   VerticalDownEnd
};

enum class ExplosionSound
{
   None,
   Booom
};

enum class ExplosionAnimation
{
   Spawn,
   Burn,
   Destroy
};

class Explosion : public ArenaObject, public EventListener
{
public:
   Explosion(const std::shared_ptr<Arena>& arena,
             ExplosionType type,
             PlayerType owner,
             EventQueue& queue);
   virtual ~Explosion();

   Explosion(const Explosion&) = delete;
   Explosion& operator=(const Explosion&) = delete;

   void Update(int elapsed_time) override;
   void OnEvent(const Event& event) override;

   ExplosionType GetType() const;
   ExplosionAnimation GetAnimation() const;
   PlayerType GetOwner() const;
   ExplosionSound GetSound(bool reset);

private:
   void OnSpawnExplosionStart(const SpawnExplosionStartEvent& event);
   void OnSpawnExplosionEnd(const SpawnExplosionEndEvent& event);
   void OnDestroyExplosionStart(const DestroyExplosionStartEvent& event);
   void OnDestroyExplosionEnd(const DestroyExplosionEndEvent& event);

   const ExplosionType mType;
   const PlayerType mOwner;

   EventQueue& mEventQueue;

   ExplosionAnimation mAnimation = ExplosionAnimation::Spawn;
   ExplosionSound mSound = ExplosionSound::None;
};

#endif // EXPLOSION_HPP
