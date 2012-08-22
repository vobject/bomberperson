#ifndef WALL_HPP
#define WALL_HPP

#include "SceneObject.hpp"

enum class WallType
{
//   Crossway,
//   Vertical,
//   VerticalLeftEnd,
//   VerticalRightEnd,
//   Horizontal,
//   HorizontalLeftEnd,
//   HorizontalRightEnd,
//   NorthWestCorner,
//   NorthEastCorner,
//   SouthWestCorner,
//   SouthEastCorner

   Indestructible,
   Destructible
};

class Wall : public SceneObject
{
public:
   Wall(const std::string& name, WallType type);
   virtual ~Wall();

   Wall(const Wall&) = delete;
   Wall& operator=(const Wall&) = delete;

   void Update(int elapsed_time) override;

   WallType GetType() const;
   bool IsDestructible() const;

private:
   WallType mType;
};

#endif // WALL_HPP
