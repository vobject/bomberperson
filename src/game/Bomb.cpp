#include "Bomb.hpp"
#include "Arena.hpp"
#include "EntityManager.hpp"
#include "Wall.hpp"
#include "Explosion.hpp"
#include "../Options.hpp"

Bomb::Bomb(const std::shared_ptr<Arena>& arena, EntityManager& entity_factory)
   : ArenaObject(EntityId::Bomb, ZOrder::Layer_4, arena)
   , mEntityFactory(entity_factory)
{
   mSound = BombSound::Planted;
}

Bomb::~Bomb()
{

}

void Bomb::Update(const int elapsed_time)
{
   SetAnimationTime(GetAnimationTime() + elapsed_time);

   if (IsValid() && (GetAnimationTime() >= DefaultValue::BOMB_ANIM_LENGTH))
   {
      // Lifetime of this bomb object ended...
      Invalidate();

      // ... instead it creates some explosions around it.
      PlantCenterExplosion();
      PlantRangeExplosion(Direction::Up);
      PlantRangeExplosion(Direction::Down);
      PlantRangeExplosion(Direction::Left);
      PlantRangeExplosion(Direction::Right);
   }
}

BombSound Bomb::GetSound(const bool reset)
{
   const auto ret = mSound;

   if (reset) {
      mSound = BombSound::None;
   }
   return ret;
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
   SetAnimationTime(DefaultValue::BOMB_ANIM_LENGTH);
}

void Bomb::PlantCenterExplosion() const
{
   const auto parent = GetArena()->GetCellFromObject(*this);
   auto explosion = mEntityFactory.CreateExplosion(parent);
   explosion->SetSound();
   GetArena()->SetExplosion(parent, explosion);
}

void Bomb::PlantRangeExplosion(Direction dir) const
{
   const auto parent = GetArena()->GetCellFromObject(*this);
   auto range_cell = GetArena()->GetNeighborCell(parent, dir);
   auto range_to_go = GetRange();

   while ((-1 != range_cell.X) && (-1 != range_cell.Y) && range_to_go)
   {
      if (GetArena()->HasWall(range_cell) &&
          !GetArena()->GetWall(range_cell)->IsDestructible())
      {
         // A wall that is not destructible is, well ... indestructible.
         // Do not spread the explosion in this direction any further.
         break;
      }

      if (GetArena()->HasBomb(range_cell))
      {
         // A bombs explosion range ends if it hits another bomb it its way.
         // But it causes the other bomb to explode.
         GetArena()->DetonateBomb(range_cell);
         break;
      }

      // TODO: Select the right ExplosionType (horizontal, vertical, etc).
      auto range_exp = mEntityFactory.CreateExplosion(range_cell);
      GetArena()->SetExplosion(range_cell, range_exp);

      if (GetArena()->HasWall(range_cell) &&
          GetArena()->GetWall(range_cell)->IsDestructible())
      {
         // FIXME: This has a problem: when the exploding bomb destroys
         //  a wall and triggers another bomb with a greater range,
         //  the wall behind the first wall would also be destroyed
         //  at the same time.
         // Maybe this could be avoided by introducing an ExplodingWall type.
         GetArena()->DestroyWall(range_cell);
         break;
      }

      if (GetArena()->HasExtra(range_cell))
      {
         // The explosion can destroy an extra item but it will be stopped
         //  when doing so.
         GetArena()->DestroyExtra(range_cell);
         break;
      }

      range_cell = GetArena()->GetNeighborCell(range_cell, dir);
      range_to_go--;
   }
}
