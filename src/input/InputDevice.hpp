#ifndef INPUT_DEVICE_HPP
#define INPUT_DEVICE_HPP

struct InputCommands
{
   constexpr InputCommands()
      : up{false}
      , down{false}
      , left{false}
      , right{false}
      , action1{false}
      , action2{false} { }

   constexpr InputCommands(const bool up_,
                           const bool down_,
                           const bool left_,
                           const bool right_,
                           const bool action1_,
                           const bool action2_)
      : up{up_}
      , down{down_}
      , left{left_}
      , right{right_}
      , action1{action1_}
      , action2{action2_} { }

   bool up;
   bool down;
   bool left;
   bool right;
   bool action1;
   bool action2;
};

class InputDevice
{
public:
   InputDevice() { }
   virtual ~InputDevice() { }

   InputCommands GetCommands() const
   {
      return { TestUp(),
               TestDown(),
               TestLeft(),
               TestRight(),
               TestAction1(),
               TestAction2() };
   }

protected:
   virtual bool TestUp() const = 0;
   virtual bool TestDown() const = 0;
   virtual bool TestLeft() const = 0;
   virtual bool TestRight() const = 0;
   virtual bool TestAction1() const = 0;
   virtual bool TestAction2() const = 0;
};

#endif // INPUT_DEVICE_HPP
