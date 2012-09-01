#include "SceneObject.hpp"

SceneObject::SceneObject(const EntityId id)
   : mId(id)
   , mPos(0, 0)
   , mSize(0, 0)
//   , mZOrder(ZOrder::zo_Layer_1)
//   , mIsVisible(true)
   , mIsAlive(true)
{

}

SceneObject::~SceneObject()
{

}

//bool SceneObject::operator<(const SceneObject& other)
//{
//   return GetZOrder() < other.GetZOrder();
//}

EntityId SceneObject::GetId() const
{
   return mId;
}

Point SceneObject::GetPosition() const
{
   return mPos;
}

void SceneObject::SetPosition(const Point& pos)
{
   mPos = pos;
}

Size SceneObject::GetSize() const
{
   return mSize;
}

void SceneObject::SetSize(const Size& size)
{
   mSize = size;
}

//ZOrder SceneObject::GetZOrder() const
//{
//   return mZOrder;
//}
//
//void SceneObject::SetZOrder(const ZOrder order)
//{
//   mZOrder = order;
//}
//
//bool SceneObject::IsVisible() const
//{
//   return mIsVisible;
//}
//
//void SceneObject::SetVisible(const bool visible)
//{
//   mIsVisible = visible;
//}

bool SceneObject::IsAlive() const
{
   return mIsAlive;
}

void SceneObject::SetAlive(const bool alive)
{
   mIsAlive = alive;
}
