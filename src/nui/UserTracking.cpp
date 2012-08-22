#include "UserTracking.hpp"
#include "../utils/Utils.hpp"

#if defined(USE_OPENNI)
#include <array>

// Callback functions for xn::UserGenerator class.
void XN_CALLBACK_TYPE UserFoundCB(xn::UserGenerator& gen, XnUserID id, void* cookie);
void XN_CALLBACK_TYPE UserLostCB(xn::UserGenerator& gen, XnUserID id, void* cookie);

// Callback functions for xn::PoseDetectionCapability class.
void XN_CALLBACK_TYPE PoseStartCB(xn::PoseDetectionCapability& gen,
                                  const XnChar* pose, XnUserID id, void* cookie);
void XN_CALLBACK_TYPE PoseEndCB(xn::PoseDetectionCapability& gen,
                                const XnChar* pose, XnUserID id, void* cookie);

// Callback functions for xn::SkeletonCapability class.
void XN_CALLBACK_TYPE CalibStartCB(xn::SkeletonCapability& gen,
                                   XnUserID id, void* cookie);
void XN_CALLBACK_TYPE CalibEndCB(xn::SkeletonCapability& gen, XnUserID id,
                                 XnCalibrationStatus calibrationError, void* cookie);

UserTracking::UserTracking()
{

}

UserTracking::~UserTracking()
{

}

void UserTracking::Init(xn::Context& ctx)
{
   if (XN_STATUS_OK != mUserGen.Create(ctx)) {
      throw "xn::UserGenerator could not be inizialized. Does the context support it?";
   }

   mCallbackArgs.user_gen = &mUserGen;

   // Callback methods for the xn::UserGenerator class.
   mUserGen.RegisterUserCallbacks(UserFoundCB, UserLostCB, &mCallbackArgs, mUserGenCB);

   xn::PoseDetectionCapability pose_cap = mUserGen.GetPoseDetectionCap();
   pose_cap.RegisterToPoseDetected(PoseStartCB, &mCallbackArgs, mPoseStartCB);
   pose_cap.RegisterToOutOfPose(PoseEndCB, &mCallbackArgs, mPoseEndCB);

   xn::SkeletonCapability skel_cap = mUserGen.GetSkeletonCap();
   skel_cap.RegisterToCalibrationStart(CalibStartCB, &mCallbackArgs, mCalibStartCB);
   skel_cap.RegisterToCalibrationComplete(CalibEndCB, &mCallbackArgs, mCalibEndCB);
   skel_cap.SetSkeletonProfile(XN_SKEL_PROFILE_ALL);
}

std::vector<UserData> UserTracking::GetUsers(const xn::DepthGenerator& gen) const
{
   XnUInt16 userMax = 16;
   std::array<XnUserID, 16> userBuf;

   if (userMax < mUserGen.GetNumberOfUsers()) {
      LOG(logWARNING) << mUserGen.GetNumberOfUsers() << " users detected"
                      << " but we only support " << userMax;
   }

   std::vector<UserData> users;
   mUserGen.GetUsers(userBuf.data(), userMax);

   for (XnUInt16 id = 0; userMax >= id; id++)
   {
      if (!mUserGen.GetSkeletonCap().IsTracking(userBuf[id])) {
         // We are only interested in the users that are being tracked.
         continue;
      }

      UserData user(userBuf[id]);

      XnSkeletonJointPosition pos;
      XnPoint3D perspective_pos;
      for (int joint = XN_SKEL_HEAD; joint <= XN_SKEL_RIGHT_FOOT; joint++)
      {
         mUserGen.GetSkeletonCap().GetSkeletonJointPosition(userBuf[id], (XnSkeletonJoint)joint, pos);
         if (0.5 > pos.fConfidence) {
            continue;
         }
         gen.ConvertRealWorldToProjective(1, &pos.position, &perspective_pos);

         user.SetRealWorldJoint(joint, pos.position);
         user.SetPerspectiveJoint(joint, perspective_pos);
      }

      users.push_back(user);
   }
   return users;
}

std::shared_ptr<xn::SceneMetaData> UserTracking::GetUserPixels(const UserData& user) const
{
   std::shared_ptr<xn::SceneMetaData> meta(new xn::SceneMetaData());

   mUserGen.GetUserPixels(user.GetId(), *meta);
   return meta;
}

void XN_CALLBACK_TYPE UserFoundCB(
   xn::UserGenerator& gen,
   XnUserID id,
   void* /*cookie*/
)
{
   LOG(logDEBUG) << "UserFoundCB()";

   gen.GetPoseDetectionCap().StartPoseDetection("Psi", id);
}

void XN_CALLBACK_TYPE UserLostCB(
   xn::UserGenerator& /*gen*/,
   XnUserID /*id*/,
   void* /*cookie*/
)
{
   LOG(logDEBUG) << "UserLostCB()";
}

void XN_CALLBACK_TYPE PoseStartCB(
   xn::PoseDetectionCapability& /*cap*/,
   const XnChar* /*pose*/,
   XnUserID id,
   void* cookie
)
{
   LOG(logDEBUG) << "PoseStartCB()";

   UserTrackerCallbackArgs* args = static_cast<UserTrackerCallbackArgs*>(cookie);
   args->user_gen->GetPoseDetectionCap().StopPoseDetection(id);
   args->user_gen->GetSkeletonCap().RequestCalibration(id, TRUE);
}

void XN_CALLBACK_TYPE PoseEndCB(
   xn::PoseDetectionCapability& /*cap*/,
   const XnChar* /*pose*/,
   XnUserID /*id*/,
   void* /*cookie*/
)
{
   LOG(logDEBUG) << "PoseEndCB()";
}

void XN_CALLBACK_TYPE CalibStartCB(
   xn::SkeletonCapability& /*cap*/,
   XnUserID /*id*/,
   void* /*cookie*/
)
{
   LOG(logDEBUG) << "CalibStartCB()";
}

void XN_CALLBACK_TYPE CalibEndCB(
   xn::SkeletonCapability& /*cap*/,
   XnUserID id,
   XnCalibrationStatus calibrationError,
   void* cookie
)
{
   UserTrackerCallbackArgs* args = static_cast<UserTrackerCallbackArgs*>(cookie);

   if (XN_CALIBRATION_STATUS_OK == calibrationError)
   {
      LOG(logDEBUG) << "User successfully calibrated.";
      args->user_gen->GetSkeletonCap().StartTracking(id);
   }
   else
   {
      LOG(logDEBUG) << "Failed to calibrate user!";
      args->user_gen->GetPoseDetectionCap().StartPoseDetection("Psi", id);
   }
}
#endif // USE_OPENNI
