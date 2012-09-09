#ifndef BOMB_HPP
#define BOMB_HPP

#include "SceneObject.hpp"
#include "../utils/Utils.hpp"

#include <memory>

class EntityManager;
class Cell;

class Bomb : public SceneObject
{
public:
   Bomb(EntityManager& entity_factory, const std::shared_ptr<Cell>& cell);
   virtual ~Bomb();

   Bomb(const Bomb&) = delete;
   Bomb& operator=(const Bomb&) = delete;

   void Update(int elapsed_time) override;

   int GetAnimationTime() const;

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

   std::shared_ptr<Cell> mParentCell;
   int mLifeTime = 0_ms;
   int mRange = 1;

//   Animation mAnimation;
};

#endif // BOMB_HPP
