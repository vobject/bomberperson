#ifndef USER_DATA_HPP
#define USER_DATA_HPP

#if defined(USE_OPENNI)
#include "openni_all.h"

#include <map>
#include <string>

class UserData
{
public:
   static std::string NameFromId(int joint_id);
   static const int INVALID_USER_ID = -1;

   UserData(XnUserID id);
   ~UserData();

   XnUserID GetId() const;

   std::map<int, XnPoint3D> GetRealWorldJoints() const;
   std::map<int, XnPoint3D> GetPerspectiveJoints() const;

   void SetRealWorldJoint(int joint, const XnPoint3D& pos);
   void SetPerspectiveJoint(int joint, const XnPoint3D& pos);

private:
   static const std::map<int, std::string> mJointNames;

   XnUserID mId;
   std::map<int, XnPoint3D> mRealWorldJoints;
   std::map<int, XnPoint3D> mPerspectiveJoints;
};
#endif // USE_OPENNI

#endif // USER_DATA_HPP
