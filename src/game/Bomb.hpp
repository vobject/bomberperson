#ifndef BOMB_HPP
#define BOMB_HPP

#include "ArenaObject.hpp"
#include "../utils/Utils.hpp"

class EntityManager;

enum class BombType
{
   Countdown,
   Remote
};

enum class BombSound
{
   None,
   Planted
};

class Bomb : public ArenaObject
{
public:
   Bomb(const std::shared_ptr<Arena>& arena,
        BombType type,
        EntityManager& entity_factory);
   virtual ~Bomb();

   Bomb(const Bomb&) = delete;
   Bomb& operator=(const Bomb&) = delete;

   void Update(int elapsed_time) override;

   BombType GetType() const;
   BombSound GetSound(bool reset);

   int GetRange() const;
   void SetRange(int range);

   void Detonate();

private:
   void PlantCenterExplosion() const;
   void PlantRangeExplosion(Direction dir) const;

   const BombType mType;
   EntityManager& mEntityFactory;

   int mLifeTime = 0_ms;
   int mRange = 1;
   BombSound mSound = BombSound::None;

};

#endif // BOMB_HPP
