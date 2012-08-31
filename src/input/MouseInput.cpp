#include "MouseInput.hpp"

MouseInput::MouseInput(const Point center, int bomb_keycode)
   : mCenter(center)
   , mBombKey(bomb_keycode)
{
}

MouseInput::~MouseInput()
{

}

bool MouseInput::TestUp() const
{
   return mMoveUp;
}

bool MouseInput::TestDown() const
{
   return mMoveDown;
}

bool MouseInput::TestLeft() const
{
   return mMoveLeft;
}

bool MouseInput::TestRight() const
{
   return mMoveRight;
}

bool MouseInput::TestPlantBomb() const
{
   return mBombKeyPressed;
}

void MouseInput::Press(const int key)
{
   if (key == mBombKey)
   {
      mBombKeyPressed = true;
   }
}

void MouseInput::Release(const int key)
{
   if (key == mBombKey)
   {
      mBombKeyPressed = false;
   }
}

void MouseInput::Move(const Point pt)
{
   if (pt.Y < mCenter.Y) {
      mMoveUp = true;
      mMoveDown = false;
   }
   else if (pt.Y > mCenter.Y) {
      mMoveUp = false;
      mMoveDown = true;
   }
   else {
      mMoveUp = false;
      mMoveDown = false;
   }

   if (pt.X < mCenter.X) {
      mMoveLeft = true;
      mMoveRight = false;
   }
   else if (pt.X > mCenter.X) {
      mMoveLeft = false;
      mMoveRight = true;
   }
   else {
      mMoveLeft = false;
      mMoveRight = false;
   }
}
