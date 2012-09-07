#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP

#include "EntityId.hpp"
#include "../utils/Utils.hpp"

#include <string>

//enum class ZOrder
//{
//   zo_Background,
//   zo_Layer_1,
//   zo_Layer_2,
//   zo_Layer_3,
//   zo_Messagebox
//};

class SceneObject
{
public:
   SceneObject(EntityId id);
   virtual ~SceneObject();

//   virtual bool operator<(const SceneObject& other);

   virtual void Update(int elapsed_time) = 0;
//   virtual void Restore() = 0;

   EntityId GetId() const;

   Point GetPosition() const;
   void SetPosition(const Point& pos);

   Size GetSize() const;
   void SetSize(const Size& size);

//   ZOrder GetZOrder() const;
//   void SetZOrder(ZOrder order);

//   bool IsVisible() const;
//   void SetVisible(bool visible);

   bool IsAlive() const;
   void SetAlive(bool alive);

//   virtual SceneObject* Create() = 0;
//   virtual SceneObject* Clone() = 0;
//   virtual void Destroy();

private:
   EntityId mId;
   Point mPos;
   Size mSize;
//   ZOrder mZOrder;
//   bool mIsVisible;
   bool mIsAlive;

   // Do we really need Size in Gamelogic? Isnt a simple point enough?
   // RenderInfo -> could also contain animation info
   // SoundInfo
   // Create Render and Audio objects out of this gamelogic objects.
};

#endif // SCENE_OBJECT_HPP
