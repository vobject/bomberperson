#ifndef WALL_HPP
#define WALL_HPP

#include "ArenaObject.hpp"

//enum class WallType
//{
////   Crossway,
////   Vertical,
////   VerticalLeftEnd,
////   VerticalRightEnd,
////   Horizontal,
////   HorizontalLeftEnd,
////   HorizontalRightEnd,
////   NorthWestCorner,
////   NorthEastCorner,
////   SouthWestCorner,
////   SouthEastCorner

//   Indestructible,
//   Destructible
//};

enum class WallType
{
   Indestructible,
   Destructible
};

enum class WallSound
{

};

class Wall : public ArenaObject
{
public:
   Wall(const std::shared_ptr<Arena>& arena, WallType type);
   virtual ~Wall();

   Wall(const Wall&) = delete;
   Wall& operator=(const Wall&) = delete;

   void Update(int elapsed_time) override;

   WallType GetType() const;
   bool IsDestructible() const;

private:
   const WallType mType;
};

#endif // WALL_HPP
