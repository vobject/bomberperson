#ifndef NUI_HPP
#define NUI_HPP

#if defined(USE_OPENNI)
#include "UserData.hpp"
#include "../utils/Utils.hpp"

#include "openni_all.h"

#include <memory>
#include <vector>

// NUI (Natural User Interface) abstract class
class Nui
{
public:
   Nui() : mRes(0, 0) { }
   virtual ~Nui() { }

   virtual void Init() = 0;
   virtual void NextFrame() = 0;

   virtual const XnRGB24Pixel* GetImageData() = 0;
   virtual const XnDepthPixel* GetDepthData() = 0;

   virtual std::vector<UserData> GetUsers() const = 0;
   virtual std::shared_ptr<xn::SceneMetaData> GetUserPixels(const UserData& user) const = 0;

   Size GetSize() const { return mRes; }

   // TODO:
   //  OnGesture(GESTURE, callback);

protected:
   Size mRes;
};
#endif // USE_OPENNI

#endif // NUI_HPP
