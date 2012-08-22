#ifndef WINDOW_FRAME_HPP
#define WINDOW_FRAME_HPP

#include "utils/Utils.hpp"
#include "Options.hpp"

#include <SDL.h>

#include <string>

class WindowFrame
{
public:
   WindowFrame(const std::string& title);
   ~WindowFrame();

   void UpdateDone();
   void FrameDone();

protected:
   static Uint32 DefaultFrameTimerCallback(Uint32 interval, void *param);

   std::string mTitle;

   const int mFrameTimerUpdateRate = 1000_ms;
   SDL_TimerID mFrameTimer = nullptr;

   int mUpdateCount = 0; // Internal game updates (input, physics, etc.)
   int mFrameCount = 0; // Rendered frames.

   int mUPS = 0;
   int mFPS = 0;
};

#endif // WINDOW_FRAME_HPP
