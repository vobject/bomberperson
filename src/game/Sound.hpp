#ifndef SOUND_HPP
#define SOUND_HPP

#include "../utils/Utils.hpp"

class Sound
{
public:
   Sound();
   ~Sound();

   Sound(const Sound&) = delete;
   Sound& operator=(const Sound&) = delete;

//   void SetFrameCount(int n);
//   void SetLength(int milliseconds);
//   void SetLooping(bool endless_loop);
//
//   void Update(int elapsed_time);
//   void Reset();
//   int GetCurrentFrame() const;

private:
//   int mFrameCount = 0;
//   int mLength = 0_ms; // Will crash when not initialized. But we want that!
//   bool mLooping = false;
//   int mElapsedTime = 0;
};

#endif // SOUND_HPP
