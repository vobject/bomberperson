#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "ArenaObject.hpp"

class Player;

enum class ExplosionType
{
   Center,
   Vertical,
   VerticalLeftEnd,
   VerticalRightEnd,
   Horizontal,
   HorizontalUpEnd,
   HorizontalDownEnd
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
             const std::shared_ptr<Player>& owner);
   virtual ~Explosion();

   Explosion(const Explosion&) = delete;
   Explosion& operator=(const Explosion&) = delete;

   void Update(int elapsed_time) override;

   ExplosionType GetType() const;
   std::shared_ptr<Player> GetOwner() const;
   ExplosionSound GetSound(bool reset);

private:
   const ExplosionType mType;
   const std::shared_ptr<Player> mOwner;
   ExplosionSound mSound = ExplosionSound::None;
};

#endif // EXPLOSION_HPP
