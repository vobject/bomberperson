#ifndef CELL_HPP
#define CELL_HPP

//#include "ArenaObject.hpp"

//class Wall;
//class Extra;
//class Bomb;
//class Explosion;

//// TODO: Is a cell class really necessary??
//// All ArenaObject objects have a pointer to the underlying arena.
//// So they could plant their new objects directly into the arena.
//// The parent cell could consist of only (X,Y) coordinates.
//// GetNeighborCell() is redirected to the arena object anyway.
//// The arena would have to manage the arenaobjects for each cell then.

//class Cell : public ArenaObject
//{
//public:
//   Cell(const std::shared_ptr<Arena>& arena,
//        int arena_pos_x,
//        int arena_pos_y);
//   virtual ~Cell();

//   Cell(const Cell&) = delete;
//   Cell& operator=(const Cell&) = delete;

//   void Update(int elapsed_time) override;

//   bool HasWall() const;
//   std::shared_ptr<Wall> GetWall() const;
//   void SetWall(const std::shared_ptr<Wall>& wall);
//   void DestroyWall();

//   bool HasExtra() const;
//   std::shared_ptr<Extra> GetExtra() const;
//   void SetExtra(const std::shared_ptr<Extra>& extra);
//   void DestroyExtra();

//   bool HasBomb() const;
//   std::shared_ptr<Bomb> GetBomb() const;
//   void SetBomb(const std::shared_ptr<Bomb>& bomb);
//   void DetonateBomb();

//   bool HasExplosion() const;
//   std::shared_ptr<Explosion> GetExplosion() const;
//   void SetExplosion(const std::shared_ptr<Explosion>& explosion);

//   std::shared_ptr<Cell> GetNeighborCell(Direction dir) const;

//private:
//   // (X,Y) index on the playing field.
//   const int mXCell;
//   const int mYCell;

//   std::shared_ptr<Wall> mWall;
//   std::shared_ptr<Extra> mExtra;
//   std::shared_ptr<Bomb> mBomb;
//   std::shared_ptr<Explosion> mExplosion;
//};

#endif // CELL_HPP
