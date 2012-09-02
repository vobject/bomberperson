#include "MouseInput.hpp"

MouseInput::MouseInput(
   const Point center,
   const int action1_keycode,
   const int action2_keycode
)
   : mCenter(center)
   , mAction1Button(action1_keycode)
   , mAction2Button(action2_keycode)
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

bool MouseInput::TestAction1() const
{
   return mAction1ButtonPressed;
}

bool MouseInput::TestAction2() const
{
   return mAction2ButtonPressed;
}

void MouseInput::Press(const int button)
{
   if (button == mAction1Button)
   {
      mAction1ButtonPressed = true;
   }
   else if (button == mAction2Button)
   {
      mAction2ButtonPressed = true;
   }
}

void MouseInput::Release(const int button)
{
   if (button == mAction1Button)
   {
      mAction1ButtonPressed = false;
   }
   else if (button == mAction2Button)
   {
      mAction2ButtonPressed = false;
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
