#include "KeyboardInput.hpp"
#include "../utils/Utils.hpp"

KeyboardInput::KeyboardInput(
   const int up_keycode,
   const int down_keycode,
   const int left_keycode,
   const int right_keycode,
   const int bomb_keycode
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

void KeyboardInput::Press(const SDLKey key)
{
   if (key == mUpKey) {
      mUpKeyPressed = true;
      LOG(logDEBUG) << "Press: UP";
   }
   else if (key == mDownKey) {
      mDownKeyPressed = true;
      LOG(logDEBUG) << "Press: DOWN";
   }
   else if (key == mLeftKey) {
      mLeftKeyPressed = true;
      LOG(logDEBUG) << "Press: LEFT";
   }
   else if (key == mRightKey) {
      mRightKeyPressed = true;
      LOG(logDEBUG) << "Press: RIGHT";
   }
   else if (key == mBombKey) {
      mBombKeyPressed = true;
      LOG(logDEBUG) << "Press: BOMB";
   }
}

void KeyboardInput::Release(const SDLKey key)
{
   if (key == mUpKey) {
      mUpKeyPressed = false;
      LOG(logDEBUG) << "Release: UP";
   }
   else if (key == mDownKey) {
      mDownKeyPressed = false;
      LOG(logDEBUG) << "Release: DOWN";
   }
   else if (key == mLeftKey) {
      mLeftKeyPressed = false;
      LOG(logDEBUG) << "Release: LEFT";
   }
   else if (key == mRightKey) {
      mRightKeyPressed = false;
      LOG(logDEBUG) << "Release: RIGHT";
   }
   else if (key == mBombKey) {
      mBombKeyPressed = false;
      LOG(logDEBUG) << "Release: BOMB";
   }
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

