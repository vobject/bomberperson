#include "Bomb.hpp"
#include "EntityManager.hpp"
#include "Cell.hpp"
#include "Wall.hpp"
#include "Explosion.hpp"

Bomb::Bomb(EntityManager& entity_factory, const std::shared_ptr<Cell>& cell)
   : SceneObject(EntityId::Bomb)
   , mEntityFactory(entity_factory)
   , mParentCell(cell)
{
   SetZOrder(ZOrder::Layer_4);

   mPlantedSound = true;
}

Bomb::~Bomb()
{

}

void Bomb::Update(const int elapsed_time)
{
   SetSound(SoundId::None);

   mLifeTime += elapsed_time;

   if (IsAlive() && (mLifeTime >= DEFAULT_LIFETIME))
   {
      // Lifetime of this bomb object ended...
      SetAlive(false);

      // ... instead it creates some explosions around it.
      PlantCenterExplosion();
      PlantRangeExplosion(Direction::Up);
      PlantRangeExplosion(Direction::Down);
      PlantRangeExplosion(Direction::Left);
      PlantRangeExplosion(Direction::Right);
   }
}

int Bomb::GetAnimationTime() const
{
   return mLifeTime;
}

bool Bomb::WasPlantedSound() const
{
   if (mPlantedSound) {
      mPlantedSound = false;
      return true;
   }
   return false;
}

int Bomb::GetRange() const
{
   return mRange;
}

void Bomb::SetRange(const int range)
{
   mRange = range;
}

void Bomb::Detonate()
{
   mLifeTime = DEFAULT_LIFETIME;
}

void Bomb::PlantCenterExplosion() const
{
   auto explosion = mEntityFactory.CreateExplosion(mParentCell);
   explosion->SetSound(SoundId::Explosion);
   mParentCell->SetExplosion(explosion);
}

void Bomb::PlantRangeExplosion(Direction dir) const
{
   auto range_cell = mParentCell->GetNeighborCell(dir);
   auto range_to_go = GetRange();

   while (range_cell && range_to_go)
   {
      if (range_cell->HasWall() && !range_cell->GetWall()->IsDestructible()) {
         // A wall that is not destructible is, well ... indestructible.
         // Do not spread the explosion in this direction any further.
         break;
      }

      if (range_cell->HasBomb()) {
         // A bombs explosion range ends if it hits another bomb it its way.
         // But it causes the other bomb to explode.
         range_cell->DetonateBomb();
         break;
      }

      // TODO: Select the right ExplosionType (horizontal, vertical, etc).
      auto range_exp = mEntityFactory.CreateExplosion(range_cell);
      range_cell->SetExplosion(range_exp);

      if (range_cell->HasWall() && range_cell->GetWall()->IsDestructible())
      {
         // FIXME: This has a problem: when the exploding bomb destroys
         //  a wall and triggers another bomb with a greater range,
         //  the wall behind the first wall would also be destroyed
         //  at the same time.
         // Maybe this could be avoided by introducing an ExplodingWall type.
         range_cell->DestroyWall();
         break;
      }

      if (range_cell->HasExtra())
      {
         // The explosion can destroy an extra item but it will be stopped
         //  when doing so.
         range_cell->DestroyExtra();
         break;
      }

      range_cell = range_cell->GetNeighborCell(dir);
      range_to_go--;
   }
}
