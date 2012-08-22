#include "BomberPersonApp.hpp"
#include "WindowFrame.hpp"
#include "Options.hpp"
#include "nui/Kinect.hpp"
#include "nui/KinectDummy.hpp"
#include "render/SimpleGlRenderer.hpp"
#include "render/SimpleSdlRenderer.hpp"
#include "render/SdlRenderer.hpp"
#include "game/Logic.hpp"

#include <SDL.h>

#include <X11/Xlib.h>

BomberPersonApp::BomberPersonApp()
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

   // TODO: Replace SDL times with sd::chonos classes.
//   auto sys = std::chrono::system_clock::now();
//   auto stead = std::chrono::steady_clock::now();
//   auto high = std::chrono::high_resolution_clock::now();

   // Milliseconds to wait for a game update.
   const int delta_time = 2_ms;
   int old_time = SDL_GetTicks();
   int game_time = 0;
   int accumulator = 0;

   while(!mQuitRequested)
   {
      const int new_time = SDL_GetTicks();
      int frame_time = new_time - old_time;

      old_time = new_time;
      accumulator += frame_time;

      while (accumulator >= delta_time)
      {
         ProcessInput();
         UpdateScene(game_time, delta_time);
         accumulator -= delta_time;
         game_time += delta_time;
      }

      RenderScene();
   }
}

void BomberPersonApp::Initialize()
{
   // HACK for ubuntu1204: https://github.com/DrMcCoy/xoreos/commit/9a6c84d5458256ac5a0ff7525055ef2d8761e683
   if (!XInitThreads()) {
      throw "Failed to initialize Xlib muti-threading support";
   }

   InitNui();

   const Size screen_size = { DefaultSize::SCREEN_WIDTH, DefaultSize::SCREEN_HEIGHT };
   mRenderer = std::make_shared<SdlRenderer>(screen_size);
   mWndFrame = std::make_shared<WindowFrame>("BomberPerson");
   mLogic = std::make_shared<Logic>(mRenderer);
}

void BomberPersonApp::ProcessInput()
{
   SDL_Event event;

   if (!SDL_PollEvent(&event)) {
      return;
   }

   if((SDL_QUIT == event.type) || (SDLK_ESCAPE == event.key.keysym.sym)) {
      // The user closed the window or pressed ESC.
      mQuitRequested = true;
      return;
   }

   switch (event.type)
   {
      case SDL_KEYDOWN:
      case SDL_KEYUP:
         {
            // TODO: Enable switching renderers with Strg-ANYKEY keys.

//            if (SDLK_s == event.key.keysym.sym) {
//               mCurrentVideoMode = VideoMode::Software;
//               InitVideo();
//               SelectRenderer();
//            }
//            else if (SDLK_o == event.key.keysym.sym) {
//               mCurrentVideoMode = VideoMode::OpenGL;
//               InitVideo();
//               SelectRenderer();
//            }

            mLogic->ProcessInput(event.key);
         }
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
   mWndFrame->UpdateDone();
}

void BomberPersonApp::RenderScene()
{
   mLogic->Render();
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
