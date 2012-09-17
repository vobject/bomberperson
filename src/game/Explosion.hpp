#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "ArenaObject.hpp"

class Player;

//enum class ExplosionType
//{

//};

enum class ExplosionSound
{
   None,
   Booom
};

class Explosion : public ArenaObject
{
public:
   Explosion(const std::shared_ptr<Arena>& arena,
             const std::shared_ptr<Player>& owner);
   virtual ~Explosion();

   Explosion(const Explosion&) = delete;
   Explosion& operator=(const Explosion&) = delete;

   void Update(int elapsed_time) override;

   std::shared_ptr<Player> GetOwner() const;
   ExplosionSound GetSound(bool reset);

   // FIXME: Introduce different ExplosionTypes and only make a sound
   //  the CenterType of the explosion!
   // For now we work around this.
   void SetSound();

private:
   const std::shared_ptr<Player> mOwner;
   ExplosionSound mSound = ExplosionSound::None;
};

#endif // EXPLOSION_HPP
