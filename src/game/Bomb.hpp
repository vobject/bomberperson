#ifndef BOMB_HPP
#define BOMB_HPP

#include "ArenaObject.hpp"
#include "../utils/Utils.hpp"

class EntityManager;

//enum class BombType
//{

//};

enum class BombSound
{
   None,
   Planted
};

class Bomb : public ArenaObject
{
public:
   Bomb(const std::shared_ptr<Arena>& arena, EntityManager& entity_factory);
   virtual ~Bomb();

   Bomb(const Bomb&) = delete;
   Bomb& operator=(const Bomb&) = delete;

   void Update(int elapsed_time) override;

   BombSound GetSound(bool reset);

   int GetRange() const;
   void SetRange(int range);

   void Detonate();

private:
   void PlantCenterExplosion() const;
   void PlantRangeExplosion(Direction dir) const;

   EntityManager& mEntityFactory;

   BombSound mSound = BombSound::None;
   int mRange = 1;

};

#endif // BOMB_HPP
