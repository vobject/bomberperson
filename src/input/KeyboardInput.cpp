#include "KeyboardInput.hpp"
#include "../utils/Utils.hpp"

KeyboardInput::KeyboardInput(
   const SDLKey up_keycode,
   const SDLKey down_keycode,
   const SDLKey left_keycode,
   const SDLKey right_keycode,
   const SDLKey bomb_keycode
)
   : mUpKey(up_keycode)
   , mDownKey(down_keycode)
   , mLeftKey(left_keycode)
   , mRightKey(right_keycode)
   , mBombKey(bomb_keycode)
{

}

KeyboardInput::~KeyboardInput()
{

}

bool KeyboardInput::TestUp() const
{
   return mUpKeyPressed;
}

bool KeyboardInput::TestDown() const
{
   return mDownKeyPressed;
}

bool KeyboardInput::TestLeft() const
{
   return mLeftKeyPressed;
}

bool KeyboardInput::TestRight() const
{
   return mRightKeyPressed;
}

bool KeyboardInput::TestPlantBomb() const
{
   return mBombKeyPressed;
}

void KeyboardInput::Press(const SDLKey key)
{
   if (key == mUpKey)
   {
      mUpKeyPressed = true;
   }
   else if (key == mDownKey)
   {
      mDownKeyPressed = true;
   }
   else if (key == mLeftKey)
   {
      mLeftKeyPressed = true;
   }
   else if (key == mRightKey)
   {
      mRightKeyPressed = true;
   }
   else if (key == mBombKey)
   {
      mBombKeyPressed = true;
   }
}

void KeyboardInput::Release(const SDLKey key)
{
   if (key == mUpKey)
   {
      mUpKeyPressed = false;
   }
   else if (key == mDownKey)
   {
      mDownKeyPressed = false;
   }
   else if (key == mLeftKey)
   {
      mLeftKeyPressed = false;
   }
   else if (key == mRightKey)
   {
      mRightKeyPressed = false;
   }
   else if (key == mBombKey)
   {
      mBombKeyPressed = false;
   }
}
