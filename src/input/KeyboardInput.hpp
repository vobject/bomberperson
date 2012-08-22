#ifndef KEYBOARD_INPUT_HPP
#define KEYBOARD_INPUT_HPP

#include "InputDevice.hpp"

#include <SDL_keysym.h>

class KeyboardInput : public InputDevice
{
public:
   KeyboardInput(int up_keycode,
                 int down_keycode,
                 int left_keycode,
                 int right_keycode,
                 int bomb_keycode);
   virtual ~KeyboardInput();

   void Press(SDLKey key) override;
   void Release(SDLKey key) override;

   bool TestUp() const override;
   bool TestDown() const override;
   bool TestLeft() const override;
   bool TestRight() const override;
   bool TestPlantBomb() const override;

private:
   const int mUpKey;
   const int mDownKey;
   const int mLeftKey;
   const int mRightKey;
   const int mBombKey;

   bool mUpKeyPressed = false;
   bool mDownKeyPressed = false;
   bool mLeftKeyPressed = false;
   bool mRightKeyPressed = false;
   bool mBombKeyPressed = false;
};

#endif // KEYBOARD_INPUT_HPP
