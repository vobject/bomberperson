#ifndef BOMB_HPP
#define BOMB_HPP

#include "SceneObject.hpp"
#include "Animation.hpp"
//#include "Sound.hpp"
#include "../utils/Utils.hpp"

#include <memory>

class Cell;

class Bomb : public SceneObject
{
public:
   Bomb(const std::string& name, const std::shared_ptr<Cell>& cell);
   virtual ~Bomb();

   Bomb(const Bomb&) = delete;
   Bomb& operator=(const Bomb&) = delete;

   void Update(int elapsed_time) override;

   int GetAnimationFrame() const;
   bool WasPlantedSound() const;

   int GetRange() const;
   void SetRange(int range);

   void Detonate();

private:
   static const int DEFAULT_LIFETIME = 2500_ms;

   void PlantCenterExplosion() const;
   void PlantRangeExplosion(Direction dir) const;

   const std::shared_ptr<Cell> mParentCell;
   int mLifeTime = 0;
   int mRange = 1;

   Animation mAnimation;
   mutable bool mPlantedSound;
//   Sound mPlantedSound;
};

#endif // BOMB_HPP
