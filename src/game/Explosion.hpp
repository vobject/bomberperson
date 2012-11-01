#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "ArenaObject.hpp"
#include "EventListener.hpp"

class EventQueue;
class RemoveExplosionEvent;

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

//enum class ExplosionAnimation
//{
////   Spawn,
////   Burn,
////   Destroy
//};

class Explosion : public ArenaObject, public EventListener
{
public:
   Explosion(const std::shared_ptr<Arena>& arena,
             ExplosionType type,
             int lifespan,
             PlayerType owner,
             EventQueue& queue);
   virtual ~Explosion();

   Explosion(const Explosion&) = delete;
   Explosion& operator=(const Explosion&) = delete;

   void Update(int elapsed_time) override;
   void OnEvent(const Event& event) override;

   ExplosionType GetType() const;
   PlayerType GetOwner() const;
   ExplosionSound GetSound(bool reset);

private:
   void OnRemoveExplosion(const RemoveExplosionEvent& event);

   const ExplosionType mType;
   const int mLifeSpan;
   const PlayerType mOwner;

   EventQueue& mEventQueue;

   ExplosionSound mSound = ExplosionSound::None;
};

#endif // EXPLOSION_HPP
