#ifndef BOMBERPERSON_APP_HPP
#define BOMBERPERSON_APP_HPP

#include "utils/Utils.hpp"

#include <memory>

#if defined(USE_OPENNI)
class Nui;
#endif // USE_OPENNI
class WindowFrame;
class Renderer;
class Logic;

enum class VideoMode
{
   Software,
   OpenGL
};

class BomberPersonApp
{
public:
   BomberPersonApp();
   ~BomberPersonApp();

   BomberPersonApp(const BomberPersonApp&) = delete;
   BomberPersonApp& operator=(const BomberPersonApp&) = delete;

   void Start();

private:
   void Mainloop();

   void Initialize();
   void ProcessInput();
   void UpdateScene(int app_time, int elapsed_time);
   void RenderScene();

   void InitNui();

   bool mQuitRequested = false;

#if defined(USE_OPENNI)
   std::shared_ptr<Nui> mNui;
#endif // USE_OPENNI
   std::shared_ptr<WindowFrame> mWndFrame;
   std::shared_ptr<Renderer> mRenderer;
   std::shared_ptr<Logic> mLogic;
};

#endif // BOMBERPERSON_APP_HPP
