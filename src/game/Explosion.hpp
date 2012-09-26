#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "ArenaObject.hpp"

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

class Explosion : public ArenaObject
{
public:
   Explosion(const std::shared_ptr<Arena>& arena,
             ExplosionType type,
             PlayerType owner);
   virtual ~Explosion();

   Explosion(const Explosion&) = delete;
   Explosion& operator=(const Explosion&) = delete;

   void Update(int elapsed_time) override;

   ExplosionType GetType() const;
   PlayerType GetOwner() const;
   ExplosionSound GetSound(bool reset);

private:
   const ExplosionType mType;
   const PlayerType mOwner;
   ExplosionSound mSound = ExplosionSound::None;
};

#endif // EXPLOSION_HPP
