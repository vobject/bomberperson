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
                 SDLKey bomb_keycode);
   virtual ~KeyboardInput();

   bool TestUp() const override;
   bool TestDown() const override;
   bool TestLeft() const override;
   bool TestRight() const override;
   bool TestPlantBomb() const override;

   void Press(SDLKey key);
   void Release(SDLKey key);

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
