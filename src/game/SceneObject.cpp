#include "SceneObject.hpp"

// Do not start at 0.
unsigned int SceneObject::sCurrentInstanceId = 1024;

SceneObject::SceneObject(const EntityId id, const ZOrder zorder)
   : mInstanceId(sCurrentInstanceId++)
   , mId(id)
   , mZOrder(zorder)
{

}

SceneObject::~SceneObject()
{

}

bool SceneObject::operator<(const SceneObject& other) const
{
   return mZOrder < other.mZOrder;
}

unsigned int SceneObject::GetInstanceId() const
{
   return mInstanceId;
}

bool SceneObject::IsValid() const
{
   return mIsValid;
}

EntityId SceneObject::GetId() const
{
   return mId;
}

ZOrder SceneObject::GetZOrder() const
{
   return mZOrder;
}

//bool SceneObject::IsVisible() const
//{
//   return mIsVisible;
//}

//void SceneObject::SetVisible(const bool status)
//{
//   mIsVisible = status;
//}

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

int SceneObject::GetAnimationTime() const
{
   return mAnimationTime;
}

void SceneObject::SetAnimationTime(const int ms)
{
   mAnimationTime = ms;
}

void SceneObject::Invalidate()
{
   mIsValid = false;
}

//SoundId SceneObject::GetSound() const
//{
//   return mSoundId;
//}

//void SceneObject::SetSound(SoundId id)
//{
//   mSoundId = id;
//}
