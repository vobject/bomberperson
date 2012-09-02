#ifndef WALL_HPP
#define WALL_HPP

#include "SceneObject.hpp"

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

class Wall : public SceneObject
{
public:
   Wall(EntityId wall_id);
   virtual ~Wall();

   Wall(const Wall&) = delete;
   Wall& operator=(const Wall&) = delete;

   void Update(int elapsed_time) override;

   bool IsDestructible() const;
};

#endif // WALL_HPP
