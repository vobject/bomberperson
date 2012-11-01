#include "Logic.hpp"
#include "UserInterface.hpp"
#include "Match.hpp"
#include "SceneObject.hpp"
#include "../audio/Audio.hpp"
#include "../render/Renderer.hpp"
#include "../utils/Utils.hpp"

#include <SDL_events.h>

Logic::Logic(BomberPersonConfig& cfg)
   : mConfig(cfg)
{
   // Show the main menu when the game starts.
   mUserInterface = std::make_shared<UserInterface>(mConfig);
   mUserInterface->ShowMainMenu(false);
}

Logic::~Logic()
{

}

void Logic::ProcessInput(const SDL_KeyboardEvent& key)
{
   if (mUserInterface->IsActive())
   {
      mUserInterface->Input(key);
   }
   else
   {
      mMatch->Input(key);
   }
}

void Logic::ProcessInput(const SDL_MouseMotionEvent& motion)
{
   if (mUserInterface->IsActive())
   {
      mUserInterface->Input(motion);
   }
   else
   {
      mMatch->Input(motion);
   }
}

void Logic::ProcessInput(const SDL_MouseButtonEvent& button)
{
   if (mUserInterface->IsActive())
   {
      mUserInterface->Input(button);
   }
   else
   {
      mMatch->Input(button);
   }
}

//void Logic::ProcessInput(const kinex::Nui& kinect)
//{
////   // FIXME: This gets input data from one frame ago instead of the current one.
////   mKinectInput->Pressed(mPlayer->GetInput());
//}

void Logic::Update(const int app_time, const int elapsed_time)
{
   (void) app_time;

   if (mUserInterface->IsActive())
   {
      UpdateUserInterface(elapsed_time);
   }
   else
   {
      UpdateMatch(elapsed_time);
   }
}

void Logic::Sound(const std::shared_ptr<Audio>& audio)
{
   if (mUserInterface->IsActive())
   {
      SoundUserInterface(*audio);
   }
   else
   {
      SoundMatch(*audio);
   }
}

void Logic::Render(const std::shared_ptr<Renderer>& renderer)
{
   renderer->PreRender();

   if (mUserInterface->IsActive())
   {
      RenderUserInterface(*renderer);
   }
   else
   {
      RenderMatch(*renderer);
   }

   renderer->PostRender();
}

bool Logic::Done() const
{
   return mDone;
}

void Logic::UpdateUserInterface(int elapsed_time)
{
   if (!mUserInterface->IsDone())
   {
      mUserInterface->Update(elapsed_time);
      return;
   }

   switch (mUserInterface->GetSelection())
   {
      case UiItemId::MainMenu_NewGame:
         {
            mUserInterface->HideMainMenu();
            mMatch = std::make_shared<Match>(mConfig, mUserInterface->GetMatchSettings());
         }
         break;
      case UiItemId::MainMenu_ResumeGame:
         {
            if (mMatch)
            {
               mUserInterface->HideMainMenu();
               mMatch->Resume();
            }
            else
            {
               mUserInterface->ShowMainMenu(false);
            }
         }
         break;
      case UiItemId::MainMenu_Exit:
         mDone = true;
         break;
      default:
         LOG(logERROR) << "Logic: Unknown menu selection.";
         break;
   }
}

void Logic::UpdateMatch(int elapsed_time)
{
   if (mMatch->GameOver())
   {
      mMatch = nullptr;
      mUserInterface->ShowMainMenu(false);
      return;
   }

   if (mMatch->Pause())
   {
      mUserInterface->ShowMainMenu(true);
      return;
   }

   mMatch->Update(elapsed_time);
}

void Logic::SoundUserInterface(Audio& audio)
{
   for (const auto& ent : mUserInterface->GetEntities())
   {
      if (!ent->IsValid()) {
         continue;
      }
      audio.Play(ent);
   }
}

void Logic::SoundMatch(Audio& audio)
{
   for (const auto& ent : mMatch->GetEntities())
   {
      if (!ent->IsValid()) {
         continue;
      }
      audio.Play(ent);
   }
}

void Logic::RenderUserInterface(Renderer& renderer)
{
   for (const auto& ent : mUserInterface->GetEntities())
   {
      if (!ent->IsValid()) {
         continue;
      }
      renderer.Render(ent);
   }
}

void Logic::RenderMatch(Renderer& renderer)
{
   for (const auto& ent : mMatch->GetEntities())
   {
      if (!ent->IsValid()) {
         continue;
      }
      renderer.Render(ent);
   }
}
