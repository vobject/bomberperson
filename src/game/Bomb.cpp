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
   mAnimation.SetFrameCount(3);
   mAnimation.SetLength(DEFAULT_LIFETIME);
   mAnimation.SetLooping(true);
}

Bomb::~Bomb()
{

}

void Bomb::Update(const int elapsed_time)
{
   mLifeTime += elapsed_time;

   if (mLifeTime >= DEFAULT_LIFETIME)
   {
      SetAlive(false);

      PlantCenterExplosion();
      PlantRangeExplosion(Direction::Up);
      PlantRangeExplosion(Direction::Down);
      PlantRangeExplosion(Direction::Left);
      PlantRangeExplosion(Direction::Right);
   }

   mAnimation.Update(elapsed_time);
}

int Bomb::GetAnimationFrame() const
{
   return mAnimation.GetCurrentFrame();
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
   mParentCell->SetExplosion(explosion);
}

void Bomb::PlantRangeExplosion(Direction dir) const
{
   std::shared_ptr<Cell> range_cell = mParentCell->GetNeighborCell(dir);
   int range_to_go = GetRange();

   while (range_cell && range_to_go)
   {
      if (range_cell->HasWall() && !range_cell->GetWall()->IsDestructible()) {
         // A wall that is not destructible is, well ... indestructible.
         break;
      }

      if (range_cell->HasBomb()) {
         // A bombs explosion range ends if it hits another bomb it its way.
         // But it causes the othe bomb to explode.
         range_cell->DetonateBomb();
         break;
      }

      // TODO: Break if an existing explosion has the same orientation
      //  as this one, e.g. Vertical or Horizontal.

      // TODO: Select the right ExplosionType.
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
