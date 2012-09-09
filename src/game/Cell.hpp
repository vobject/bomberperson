#ifndef CELL_HPP
#define CELL_HPP

#include "SceneObject.hpp"

#include <memory>

class Arena;
class Wall;
class Extra;
class Bomb;
class Explosion;

class Cell : public SceneObject
{
public:
   Cell(const std::shared_ptr<Arena>& arena,
        int arena_pos_x,
        int arena_pos_y);
   virtual ~Cell();

   Cell(const Cell&) = delete;
   Cell& operator=(const Cell&) = delete;

   void Update(int elapsed_time) override;

   bool HasWall() const;
   std::shared_ptr<Wall> GetWall() const;
   void SetWall(const std::shared_ptr<Wall>& wall);
   void DestroyWall();

   bool HasExtra() const;
   std::shared_ptr<Extra> GetExtra() const;
   void SetExtra(const std::shared_ptr<Extra>& extra);
   void DestroyExtra();

   bool HasBomb() const;
   std::shared_ptr<Bomb> GetBomb() const;
   void SetBomb(const std::shared_ptr<Bomb>& bomb);
   void DetonateBomb();

   bool HasExplosion() const;
   std::shared_ptr<Explosion> GetExplosion() const;
   void SetExplosion(const std::shared_ptr<Explosion>& explosion);

   std::shared_ptr<Cell> GetNeighborCell(Direction dir) const;

private:
   // (X,Y) index on the playing field.
   const int mArenaPosX;
   const int mArenaPosY;
   const std::shared_ptr<Arena> mArena;

   std::shared_ptr<Wall> mWall;
   std::shared_ptr<Extra> mExtra;
   std::shared_ptr<Bomb> mBomb;
   std::shared_ptr<Explosion> mExplosion;
};

#endif // CELL_HPP
