#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP

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
   SceneObject();
   virtual ~SceneObject();

//   virtual bool operator<(const SceneObject& other);

   virtual void Update(int elapsed_time) = 0;
//   virtual void Restore() = 0;

   std::string GetResourceId() const;

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

protected:
   void SetResourceId(const std::string& resource);

private:
   std::string mResource;
   Point mPos;
   Size mSize;
//   ZOrder mZOrder;
//   bool mIsVisible;
   bool mIsAlive;
};

#endif // SCENE_OBJECT_HPP
