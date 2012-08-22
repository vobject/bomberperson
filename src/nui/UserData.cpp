#include "UserData.hpp"

#if defined(USE_OPENNI)
const std::map<int, std::string> UserData::mJointNames = {
   { XN_SKEL_HEAD, "HEAD" },
   { XN_SKEL_NECK, "NECK" },
   { XN_SKEL_TORSO, "TORSO" },
   { XN_SKEL_WAIST, "WAIST" },

   { XN_SKEL_LEFT_COLLAR, "LEFT_COLLAR" },
   { XN_SKEL_LEFT_SHOULDER, "LEFT_SHOULDER" },
   { XN_SKEL_LEFT_ELBOW, "LEFT_ELBOW" },
   { XN_SKEL_LEFT_WRIST, "LEFT_WRIST" },
   { XN_SKEL_LEFT_HAND, "LEFT_HAND" },
   { XN_SKEL_LEFT_FINGERTIP, "LEFT_FINGERTIP" },

   { XN_SKEL_RIGHT_COLLAR, "RIGHT_COLLAR" },
   { XN_SKEL_RIGHT_SHOULDER, "RIGHT_SHOULDER" },
   { XN_SKEL_RIGHT_ELBOW, "RIGHT_ELBOW" },
   { XN_SKEL_RIGHT_WRIST, "RIGHT_WRIST" },
   { XN_SKEL_RIGHT_HAND, "RIGHT_HAND" },
   { XN_SKEL_RIGHT_FINGERTIP, "RIGHT_FINGERTIP" },

   { XN_SKEL_LEFT_HIP, "LEFT_HIP" },
   { XN_SKEL_LEFT_KNEE, "LEFT_KNEE" },
   { XN_SKEL_LEFT_ANKLE, "LEFT_ANKLE" },
   { XN_SKEL_LEFT_FOOT, "LEFT_FOOT" },

   { XN_SKEL_RIGHT_HIP, "RIGHT_HIP" },
   { XN_SKEL_RIGHT_KNEE, "RIGHT_KNEE" },
   { XN_SKEL_RIGHT_ANKLE, "RIGHT_ANKLE" },
   { XN_SKEL_RIGHT_FOOT, "RIGHT_FOOT" },
};

std::string UserData::NameFromId(const int joint_id)
{
   return mJointNames.at(joint_id);
}

UserData::UserData(const XnUserID id)
   : mId(id)
{

}

UserData::~UserData()
{

}

XnUserID UserData::GetId() const
{
   return mId;
}

std::map<int, XnPoint3D> UserData::GetRealWorldJoints() const
{
   return mRealWorldJoints;
}

std::map<int, XnPoint3D> UserData::GetPerspectiveJoints() const
{
   return mPerspectiveJoints;
}

void UserData::SetRealWorldJoint(const int joint, const XnPoint3D& pos)
{
   mRealWorldJoints[joint] = pos;
}

void UserData::SetPerspectiveJoint(const int joint, const XnPoint3D& pos)
{
   mPerspectiveJoints[joint] = pos;
}
#endif // USE_OPENNI
