#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP

#include "EntityId.hpp"
#include "../utils/Utils.hpp"

#include <string>

enum class ZOrder
{
   Background, // Nothing atm
   Layer_1,    // Arena, Scoreboard
   Layer_2,    // Extras
   Layer_3,    // Cells, Walls
   Layer_4,    // Bombs
   Layer_5,    // Explosions
   Layer_6,    // Players
   Menu,
   MenuItem,
   OverlayMenu,
   Messagebox
};

class SceneObject
{
public:
   SceneObject(EntityId id, ZOrder zorder);
   virtual ~SceneObject();

   bool operator<(const SceneObject& other) const;

   virtual void Update(int elapsed_time) = 0;
//   virtual void Restore() = 0;

   unsigned int GetInstanceId() const;
   bool IsValid() const;
   EntityId GetId() const;
   ZOrder GetZOrder() const;

   Point GetPosition() const;
   void SetPosition(const Point& pos);

   Size GetSize() const;
   void SetSize(const Size& size);

   int GetAnimationTime() const;
   void SetAnimationTime(int ms);

   void Invalidate();

private:
   static unsigned int sCurrentInstanceId;
   const unsigned int mInstanceId;
   const EntityId mId;
   const ZOrder mZOrder;
   bool mIsValid = true;
   Point mPos = { 0, 0 };
   Size mSize = { 0, 0 };
   int mAnimationTime = 0; // No animation by default.

   // Do we really need Size in Gamelogic? Isnt a simple point enough?
   // More advanced collision detection and handling could need an objects
   //  size information - but it might be able to get those from a rescache.
   // RenderInfo -> could also contain animation info
   // SoundInfo
   // Create Render and Audio objects out of this gamelogic objects.
};

#endif // SCENE_OBJECT_HPP
