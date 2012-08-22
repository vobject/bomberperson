#ifndef KINECT_DUMMY_HPP
#define KINECT_DUMMY_HPP

#if defined(USE_OPENNI)
#include "Nui.hpp"

class KinectDummy : public Nui
{
public:
   KinectDummy() : mRgbDummyData(KINECT_XRES * KINECT_YRES),
                   mDepthDummyData(KINECT_XRES * KINECT_YRES) { mRes = {KINECT_XRES, KINECT_YRES}; }
   virtual ~KinectDummy() { }

   KinectDummy(const KinectDummy&) = delete;
   KinectDummy& operator=(const KinectDummy&) = delete;

   void Init() override { std::fill(mRgbDummyData.begin(), mRgbDummyData.end(), XnRGB24Pixel{ 0, 0, 0 });
                          std::fill(mDepthDummyData.begin(), mDepthDummyData.end(), 0); }
   void NextFrame() override { }

   const XnRGB24Pixel* GetImageData() override { return mRgbDummyData.data(); }
   const XnDepthPixel* GetDepthData() override { return mDepthDummyData.data(); }

   std::vector<UserData> GetUsers() const override { return std::vector<UserData>(); }

   // Should never be called because GetUsers()-verctor is always empty and
   //  should always be checked before calling GetUserPixels().
   std::shared_ptr<xn::SceneMetaData> GetUserPixels(const UserData& user) const override { return nullptr; }

private:
   static const int KINECT_XRES = 640;
   static const int KINECT_YRES = 480;

   std::vector<XnRGB24Pixel> mRgbDummyData;
   std::vector<XnDepthPixel> mDepthDummyData;
};
#endif // USE_OPENNI

#endif // KINECT_DUMMY_HPP
