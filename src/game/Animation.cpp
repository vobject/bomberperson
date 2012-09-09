#include "Animation.hpp"

//Animation::Animation()
//{

//}

//Animation::~Animation()
//{

//}

//void Animation::SetFrameCount(const int n)
//{
//   mFrameCount = n;
//}

//void Animation::SetLength(const int milliseconds)
//{
//   mLength = milliseconds;
//}

//void Animation::SetLooping(const bool endless_loop)
//{
//   mLooping = endless_loop;
//}

//void Animation::Update(const int elapsed_time)
//{
//   mElapsedTime += elapsed_time;
//}

//void Animation::Reset()
//{
//   mElapsedTime = 0;
//}

//int Animation::GetCurrentFrame() const
//{
//   // An animation must always consist of one frame or more.
//   // An animations length must always be > 0.

//   const int ms_per_frame = mLength / mFrameCount;
//   int current_frame_index = mElapsedTime / ms_per_frame;

//   if (!mLooping && (current_frame_index >= mFrameCount))
//   {
//      // Reached the end of the animation.
//      //  In this case always take the last frame.
//      current_frame_index = mFrameCount - 1;
//   }

//   return current_frame_index % mFrameCount;
//}
