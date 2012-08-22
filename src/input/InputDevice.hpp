#ifndef INPUT_DEVICE_HPP
#define INPUT_DEVICE_HPP

#include <SDL_keysym.h>

//enum class InputDeviceTypes
//{
//   Keyboard_1,
//   Keyboard_2,
//   Mouse,
//   Joystick,
//   Nui
//};

class InputDevice
{
public:
   InputDevice() { }
   virtual ~InputDevice() { }

   // FIXME: Using SDLKey here is not a great choice...
   //  Introduce some sort of a keymap/actionmap!
   virtual void Press(SDLKey key) = 0;
   virtual void Release(SDLKey key) = 0;

   virtual bool TestUp() const = 0;
   virtual bool TestDown() const = 0;
   virtual bool TestLeft() const = 0;
   virtual bool TestRight() const = 0;
   virtual bool TestPlantBomb() const = 0;
};

#endif // INPUT_DEVICE_HPP
