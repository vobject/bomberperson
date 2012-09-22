#include "Ogre3DRenderer.hpp"
#include "../game/UserInterface.hpp"
#include "../game/MenuItem.hpp"
#include "../game/MenuItemSelector.hpp"
#include "../game/MainMenu.hpp"
#include "../game/Arena.hpp"
#include "../game/Scoreboard.hpp"
#include "../game/Wall.hpp"
#include "../game/Extra.hpp"
#include "../game/Bomb.hpp"
#include "../game/Explosion.hpp"
#include "../game/Player.hpp"
#include "../utils/Utils.hpp"

#include <SDL.h>

#include <OgrePlugin.h>

Ogre3DRenderer::Ogre3DRenderer(const Size res)
{
   if (0 > SDL_Init(SDL_INIT_VIDEO)) {
      throw "Cannot init SDL video subsystem.";
   }
   atexit(SDL_Quit);

   const auto screen = SDL_SetVideoMode(res.Width,
                                        res.Height,
                                        0,
                                        SDL_OPENGL);
   if (!screen) {
      throw "SDL_SetVideoMode() failed.";
   }

   mRoot = new Ogre::Root("res_q1/render/Ogre3DRenderer/plugins.cfg");

   Ogre::ConfigFile cfg_file;
   cfg_file.load("res_q1/render/Ogre3DRenderer/resources.cfg");

   auto sec_iter = cfg_file.getSectionIterator();

   while (sec_iter.hasMoreElements())
   {
      Ogre::String sec_name = sec_iter.peekNextKey();
      Ogre::ConfigFile::SettingsMultiMap* settings = sec_iter.getNext();

      for (auto it = settings->begin(); it != settings->end(); it++)
      {
         Ogre::String type_name = it->first;
         Ogre::String arch_name = it->second;
         Ogre::ResourceGroupManager::getSingleton().addResourceLocation(arch_name,
                                                                        type_name,
                                                                        sec_name);
      }
   }

   auto render_system = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
   if (!render_system) {
      throw "Could not find OpenGL Rendering Subsystem.";
   }

   render_system->setConfigOption("Full Screen","No");
   render_system->setConfigOption("VSync", "No");
   render_system->setConfigOption("Video Mode","800 x 600 @ 32-bit");

   mRoot->setRenderSystem(render_system);
   mRoot->initialise(false);

   Ogre::NameValuePairList misc = { { "currentGLContext", "True" } };

   mRenderWindow = mRoot->createRenderWindow("MainRenderWindow",
                                             res.Width,
                                             res.Height,
                                             false,
                                             &misc);
   mRenderWindow->setVisible(true);

   Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

   mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "DefaultSceneManager");
   mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
   mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);

   mCamera = mSceneMgr->createCamera("DefaultCamera");
   mCamera->setPosition(0.0f, 800.0f, 800.0f);
   mCamera->lookAt(0.0f, 0.0f, 0.0f);
   mCamera->setNearClipDistance(5.0f);

   mViewPort = mRenderWindow->addViewport(mCamera);
   mViewPort->setBackgroundColour(Ogre::ColourValue(0.0f, 0.0f, 0.0f));

   Ogre::Light* light = mSceneMgr->createLight("MainLight");
   light->setType(Ogre::Light::LT_POINT);
   light->setPosition(0.0f, 150.0f, 250.0f);
   light->setDiffuseColour(1.0f, 0.0f, 0.0f);
   light->setSpecularColour(1.0f, 0.0f, 0.0f);

   auto head_entity_1 = mSceneMgr->createEntity("Head1", "ogrehead.mesh");
   mHeadNode1 = mSceneMgr->getRootSceneNode()-> createChildSceneNode("HeadNode1", { 0.0f, 40.0f, 0.0f });
   mHeadNode1->attachObject(head_entity_1);

   auto head_entity_2 = mSceneMgr->createEntity("Head2", "ogrehead.mesh");
   mHeadNode2 = mSceneMgr->getRootSceneNode()-> createChildSceneNode("HeadNode2", { 100.0f, 40.0f, 0.0f });
   mHeadNode2->attachObject(head_entity_2);

   Ogre::MeshManager::getSingleton()
         .createPlane("MainMenuBgPlane",
                      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                      Ogre::Plane(Ogre::Vector3::UNIT_Y, 0),
                      res.Width,
                      res.Height,
                      20,
                      20,
                      true,
                      1,
                      5,
                      5,
                      Ogre::Vector3::UNIT_Z);
   auto mainmenubg_entity = mSceneMgr->createEntity("MainMenuBg", "MainMenuBgPlane");
   mainmenubg_entity->setMaterialName("Examples/Rockwall");
   mainmenubg_entity->setCastShadows(false);

   mMainMenuBgNode = mSceneMgr->getRootSceneNode()-> createChildSceneNode("MainMenuBgNode");
   mMainMenuBgNode->attachObject(mainmenubg_entity);
}

Ogre3DRenderer::~Ogre3DRenderer()
{
   delete mRoot;
}

void Ogre3DRenderer::PreRender()
{

}

void Ogre3DRenderer::PostRender()
{
   mRoot->renderOneFrame();
   SDL_GL_SwapBuffers();
}

void Ogre3DRenderer::Render(const std::shared_ptr<MenuItem>& obj)
{
}

void Ogre3DRenderer::Render(const std::shared_ptr<MenuItemSelector>& obj)
{
   const auto pos = obj->GetPosition();
   mHeadNode1->setPosition(pos.X - (mViewPort->getActualWidth() / 2),
                           40.0f,
                           pos.Y - (mViewPort->getActualHeight() / 2));
   mHeadNode1->yaw(Ogre::Degree(-0.1f));
}

void Ogre3DRenderer::Render(const std::shared_ptr<MainMenu>& obj)
{
   // TODO: Implement Me!
}

void Ogre3DRenderer::Render(const std::shared_ptr<Arena>& arena)
{
   // TODO: Implement Me!
}

void Ogre3DRenderer::Render(const std::shared_ptr<Scoreboard>& scoreboard)
{
   // TODO: Implement Me!
}

void Ogre3DRenderer::Render(const std::shared_ptr<Wall>& wall)
{
   // TODO: Implement Me!
}

void Ogre3DRenderer::Render(const std::shared_ptr<Extra>& extra)
{
   // TODO: Implement Me!
}

void Ogre3DRenderer::Render(const std::shared_ptr<Bomb>& bomb)
{
   // TODO: Implement Me!
}

void Ogre3DRenderer::Render(const std::shared_ptr<Explosion>& explosion)
{
   // TODO: Implement Me!
}

void Ogre3DRenderer::Render(const std::shared_ptr<Player>& player)
{
   // TODO: Implement Me!
}
