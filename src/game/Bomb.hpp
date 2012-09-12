#ifndef BOMB_HPP
#define BOMB_HPP

#include "ArenaObject.hpp"
#include "../utils/Utils.hpp"

class EntityManager;

//enum class BombType
//{

//};

//enum class BombSound
//{
//   None,
//   Planted
//};

class Bomb : public ArenaObject
{
public:
   Bomb(const std::shared_ptr<Arena>& arena, EntityManager& entity_factory);
   virtual ~Bomb();

   Bomb(const Bomb&) = delete;
   Bomb& operator=(const Bomb&) = delete;

   void Update(int elapsed_time) override;

   int GetAnimationTime() const;
   bool WasPlantedSound() const;

   int GetRange() const;
   void SetRange(int range);

   void Detonate();

private:
   // TODO: Keep this in sync with the animation length
   //  specified in the resource cache.
   static const int DEFAULT_LIFETIME = 2500_ms;

   void PlantCenterExplosion() const;
   void PlantRangeExplosion(Direction dir) const;

   EntityManager& mEntityFactory;

   int mLifeTime = 0_ms;
   int mRange = 1;

   mutable bool mPlantedSound = true;
//   Sound mPlantedSound;
};

#endif // BOMB_HPP
