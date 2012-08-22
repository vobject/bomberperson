#ifndef KINECT_HPP
#define KINECT_HPP

#if defined(USE_OPENNI)
#include "Nui.hpp"
#include "UserTracking.hpp"

#include <string>

class Kinect : public Nui
{
public:
   Kinect();
   virtual ~Kinect();

   Kinect(const Kinect&) = delete;
   Kinect& operator=(const Kinect&) = delete;

   void Init() override;
   void NextFrame() override;

   const XnRGB24Pixel* GetImageData() override;
   const XnDepthPixel* GetDepthData() override;

   std::vector<UserData> GetUsers() const override;
   std::shared_ptr<xn::SceneMetaData> GetUserPixels(const UserData& user) const override;

   // TODO:
   //  OnGesture(GESTURE, callback);

private:
   void InitOpenNI();

   // OpenNI members
   xn::Context mContext;
   xn::DepthGenerator mDepthGen;
   xn::ImageGenerator mImageGen;
   xn::DepthMetaData mDepthGenMD;
   xn::ImageMetaData mImageGenMD;

   UserTracking mUserTracking;
   
   bool mPlaybackMode = false;
};
#endif // USE_OPENNI

#endif // KINECT_HPP
