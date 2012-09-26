#ifndef KEYBOARD_INPUT_HPP
#define KEYBOARD_INPUT_HPP

#include "InputDevice.hpp"

#include <SDL_keysym.h>

class KeyboardInput : public InputDevice
{
public:
   KeyboardInput(SDLKey up_keycode,
                 SDLKey down_keycode,
                 SDLKey left_keycode,
                 SDLKey right_keycode,
                 SDLKey action1_keycode,
                 SDLKey action2_keycode);
   virtual ~KeyboardInput();

   void Press(SDLKey key);
   void Release(SDLKey key);

   bool TestUp() const override;
   bool TestDown() const override;
   bool TestLeft() const override;
   bool TestRight() const override;
   bool TestAction1() const override;
   bool TestAction2() const override;

private:
   const int mUpKey;
   const int mDownKey;
   const int mLeftKey;
   const int mRightKey;
   const int mAction1Key;
   const int mAction2Key;

   bool mUpKeyPressed = false;
   bool mDownKeyPressed = false;
   bool mLeftKeyPressed = false;
   bool mRightKeyPressed = false;
   bool mAction1KeyPressed = false;
   bool mAction2KeyPressed = false;
};

#endif // KEYBOARD_INPUT_HPP
