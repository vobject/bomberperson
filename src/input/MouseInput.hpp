#ifndef MOUSE_INPUT_HPP
#define MOUSE_INPUT_HPP

#include "InputDevice.hpp"
#include "../utils/Utils.hpp"

class MouseInput : public InputDevice
{
public:
   MouseInput(Point center, int action1_keycode, int action2_keycode);
   virtual ~MouseInput();

   bool TestUp() const override;
   bool TestDown() const override;
   bool TestLeft() const override;
   bool TestRight() const override;
   bool TestAction1() const override;
   bool TestAction2() const override;

   void Press(int button);
   void Release(int button);
   void Move(Point pt);

private:
   const Point mCenter;
   const int mAction1Button;
   const int mAction2Button;

   bool mMoveUp = false;
   bool mMoveDown = false;
   bool mMoveLeft = false;
   bool mMoveRight = false;
   bool mAction1ButtonPressed = false;
   bool mAction2ButtonPressed = false;
};

#endif // MOUSE_INPUT_HPP
