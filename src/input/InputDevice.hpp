#ifndef INPUT_DEVICE_HPP
#define INPUT_DEVICE_HPP

class InputDevice
{
public:
   InputDevice() { }
   virtual ~InputDevice() { }

   virtual bool TestUp() const = 0;
   virtual bool TestDown() const = 0;
   virtual bool TestLeft() const = 0;
   virtual bool TestRight() const = 0;
   virtual bool TestAction1() const = 0;
   virtual bool TestAction2() const = 0;
};

#endif // INPUT_DEVICE_HPP
