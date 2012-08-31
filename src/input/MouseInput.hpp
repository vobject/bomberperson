#ifndef MOUSE_INPUT_HPP
#define MOUSE_INPUT_HPP

#include "InputDevice.hpp"
#include "../utils/Utils.hpp"

class MouseInput : public InputDevice
{
public:
   MouseInput(Point center, int bomb_keycode);
   virtual ~MouseInput();

   bool TestUp() const override;
   bool TestDown() const override;
   bool TestLeft() const override;
   bool TestRight() const override;
   bool TestPlantBomb() const override;

   void Press(int button);
   void Release(int button);
   void Move(Point pt);

private:
   const Point mCenter;
   const int mBombKey;

   bool mMoveUp = false;
   bool mMoveDown = false;
   bool mMoveLeft = false;
   bool mMoveRight = false;
   bool mBombKeyPressed = false;
};

#endif // MOUSE_INPUT_HPP
