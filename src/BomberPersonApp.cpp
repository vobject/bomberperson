#include "BomberPersonApp.hpp"
#include "WindowFrame.hpp"
#include "nui/Kinect.hpp"
#include "nui/KinectDummy.hpp"
#include "audio/Audio.hpp"
#include "render/SimpleSdlRenderer/SimpleSdlRenderer.hpp"
#include "render/SdlRenderer/SdlRenderer.hpp"

#if defined(USE_OPENGL)
   #include "render/SimpleGlRenderer/SimpleGlRenderer.hpp"
#endif // USE_OPENGL

#if defined(USE_OGRE)
   #include "render/Ogre3DRenderer/Ogre3DRenderer.hpp"
#endif // USE_OGRE

#include "game/Logic.hpp"

#include <SDL.h>

#if !defined(_WIN32)
#include <X11/Xlib.h>
#endif

#include <chrono>

BomberPersonApp::BomberPersonApp()
   : mConfig("config.xml")
{

}

BomberPersonApp::~BomberPersonApp()
{

}

void BomberPersonApp::Start()
{
   Initialize();
   Mainloop();
}

void BomberPersonApp::Mainloop()
{
   // Mainloop based on an article from Glenn Fiedler:
   //  http://gafferongames.com/game-physics/fix-your-timestep/

   // A game update call will update the game status by this amount of time.
   const std::chrono::milliseconds delta_time(1000_ms / mConfig.GetUpdatesPerSecond());

   auto old_time = std::chrono::milliseconds(SDL_GetTicks());
   auto game_time = std::chrono::milliseconds::zero();
   auto accumulator = std::chrono::milliseconds::zero();

   while(!mQuitRequested)
   {
      const auto new_time = std::chrono::milliseconds(SDL_GetTicks());
      auto frame_time = new_time - old_time;
      old_time = new_time;

      // Number of ms the game lacks behind and has to be updated for.
      accumulator += frame_time;

      while (accumulator >= delta_time)
      {
         ProcessInput();
         UpdateScene(game_time.count(), delta_time.count());
         accumulator -= delta_time;
         game_time += delta_time;
      }
      RenderScene();
   }
}

void BomberPersonApp::Initialize()
{
#if !defined(_WIN32)
   // HACK for ubuntu1204: https://github.com/DrMcCoy/xoreos/commit/9a6c84d5458256ac5a0ff7525055ef2d8761e683
   if (!XInitThreads()) {
      throw "Failed to initialize Xlib muti-threading support";
   }
#endif

   InitNui();

   mAudio = std::make_shared<Audio>(mConfig);
   mRenderer = std::make_shared<SdlRenderer>(mConfig);
   mWndFrame = std::make_shared<WindowFrame>(mConfig.GetAppTitle());
   mLogic = std::make_shared<Logic>(mConfig);
}

void BomberPersonApp::ProcessInput()
{
   SDL_Event event;

   if (!SDL_PollEvent(&event)) {
      return;
   }

   if((SDL_QUIT == event.type) || mLogic->Done()) {
      // The user closed the window.
      mQuitRequested = true;
      return;
   }

   // Handle application-level requests, e.g. switching of renderer.
   if (SDL_KEYDOWN == event.type && (event.key.keysym.mod & KMOD_LCTRL))
   {
      if (SDLK_1 == event.key.keysym.sym) {
         mRenderer = std::make_shared<SimpleSdlRenderer>(mConfig.GetResolution());
      }
      else if (SDLK_2 == event.key.keysym.sym) {
         mRenderer = std::make_shared<SdlRenderer>(mConfig);
      }

#if defined(USE_OPENGL)
      else if (SDLK_3 == event.key.keysym.sym) {
         mRenderer = std::make_shared<SimpleGlRenderer>(mConfig.GetResolution());
      }
#endif // USE_OPENGL

#if defined(USE_OGRE)
      else if (SDLK_4 == event.key.keysym.sym) {
         mRenderer = std::make_shared<Ogre3DRenderer>(mConfig);
      }
#endif // USE_OGRE

      // TODO: KMOD_LCTRL + SDLK_K -> try connecting to kinect device.

      return;
   }

   switch (event.type)
   {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
         mLogic->ProcessInput(event.key);
         break;
      case SDL_MOUSEMOTION:
         mLogic->ProcessInput(event.motion);
         break;
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEBUTTONUP:
         mLogic->ProcessInput(event.button);
         break;
      default:
         break;
   }

//   mKinect->NextFrame();
//   mLogic->ProcessInput(*mKinect);
}

void BomberPersonApp::UpdateScene(const int app_time, const int elapsed_time)
{
   mLogic->Update(app_time, elapsed_time);
   mLogic->Sound(mAudio);
   mWndFrame->UpdateDone();
}

void BomberPersonApp::RenderScene()
{
   mLogic->Render(mRenderer);
   mWndFrame->FrameDone();
}

void BomberPersonApp::InitNui()
{
#if defined(USE_OPENNI)
   mNui = std::make_shared<Kinect>();
   try
   {
      LOG(logINFO) << "Connecting to Kinect device. Please stand by...";

      mNui->Init();
   }
   catch (...)
   {
      LOG(logINFO) << "Failed to initialize a connection to Kinect device.";

      mNui = std::make_shared<KinectDummy>();
      mNui->Init();
   }
#endif // USE_OPENNI
}
