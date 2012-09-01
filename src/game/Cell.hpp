#ifndef CELL_HPP
#define CELL_HPP

#include "SceneObject.hpp"

#include <memory>

class Arena;
class Wall;
class Extra;
class Bomb;
class Explosion;

//enum class CellType
//{
//   Floor,
//   IndestructibleWall,
//   DestructibleWall
//};
//
//enum class CellItem
//{
//   None,
//   Speed,
//   BombRange,
//   BombSupply,
//   CELL_ITEM_COUNT
//};

//class CellObject
//{
//
//};

//enum class ExtraType
//{
//   Speed,
//   BombRange,
//   BombSupply
//};
//
//class Extra : public SceneObject
//{
//
//};
//
//class Wall : public SceneObject
//{
//
//};

class Cell : public SceneObject
{
public:
   Cell(int field_pos_x,
        int field_pos_y,
        const std::shared_ptr<Arena>& field);
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
   std::shared_ptr<Extra> CollectExtra();

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
   const int mFieldPosX;
   const int mFieldPosY;
   const std::shared_ptr<Arena> mField;

   std::shared_ptr<Wall> mWall = nullptr;
   std::shared_ptr<Extra> mExtra = nullptr;
   std::shared_ptr<Bomb> mBomb = nullptr;
   std::shared_ptr<Explosion> mExplosion = nullptr;
};

#endif // CELL_HPP
