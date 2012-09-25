#include "Ogre3DRenderer.hpp"
#include "Ogre3DRenderer/ResourceCache.hpp"
#include "Ogre3DRenderer/MovableText.hpp"
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
#include "../Options.hpp"

#include <SDL.h>

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreViewport.h>
#include <OgreEntity.h>
#include <OgreMeshManager.h>

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

   mResCache = make_unique<Ogre3DResourceCache>("Ogre3DRenderer");
   mRoot = Ogre::Root::getSingletonPtr();

   // Let OGRE blindly issue OpenGL calls because we already created
   //  a context to receive them.
   Ogre::NameValuePairList params = { { "currentGLContext", "True" } };
   mRenderWindow = mRoot->createRenderWindow("RenderWindow",
                                             res.Width,
                                             res.Height,
                                             false,
                                             &params);
   mRenderWindow->setVisible(true);

   Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

   InitMenuScene();
   InitArenaScene();

////   Ogre::MovableText* msg = new Ogre::MovableText("TXT_001", "this is \nthe caption", "BlueHighway-16", 100);
////   msg->setTextAlignment(Ogre::MovableText::H_CENTER, Ogre::MovableText::V_ABOVE);
////   mHeadNode1->attachObject(msg);
}

Ogre3DRenderer::~Ogre3DRenderer()
{

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
   SelectMenuScene();
}

void Ogre3DRenderer::Render(const std::shared_ptr<MenuItemSelector>& obj)
{
   SelectMenuScene();

   const auto node_name = "MenuItemSelector";
   Ogre::SceneNode* node = nullptr;

   if (mMenuSceneMgr->hasSceneNode(node_name))
   {
      node = mMenuSceneMgr->getSceneNode(node_name);
   }
   else
   {
      auto root_node = mMenuSceneMgr->getRootSceneNode();
      auto entity = mMenuSceneMgr->createEntity("ogrehead.mesh");

      node = root_node->createChildSceneNode(node_name);
      node->attachObject(entity);
      node->pitch(Ogre::Degree(-90.0f));
      node->scale(.7f, .7f, .7f);
   }

   const auto pos = obj->GetPosition();
   node->setPosition(pos.X - (mViewPort->getActualWidth() / 2),
                     40.0f,
                     pos.Y - (mViewPort->getActualHeight() / 2));
   node->yaw(Ogre::Degree(0.1f));
}

void Ogre3DRenderer::Render(const std::shared_ptr<MainMenu>& obj)
{
   SelectMenuScene();

   const auto node_name = "MainMenuBackground";

   if (mMenuSceneMgr->hasSceneNode(node_name)) {
      return;
   }

   Ogre::MeshManager::getSingleton()
         .createPlane("MainMenuBgPlane",
                      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                      Ogre::Plane(Ogre::Vector3::UNIT_Y, 0),
                      mViewPort->getActualWidth(),
                      mViewPort->getActualHeight(),
                      20,
                      20,
                      true,
                      1,
                      5,
                      5,
                      Ogre::Vector3::UNIT_Z);

   auto entity = mMenuSceneMgr->createEntity("MainMenuBgPlane");
   entity->setMaterialName("Examples/Rockwall");
   entity->setCastShadows(false);

   auto root_node = mMenuSceneMgr->getRootSceneNode();
   auto node = root_node->createChildSceneNode(node_name);
   node->attachObject(entity);
}

void Ogre3DRenderer::Render(const std::shared_ptr<Arena>& arena)
{
   SelectArenaScene();

   const auto node_name = "Arena";

   if (mArenaSceneMgr->hasSceneNode(node_name)) {
      return;
   }

   Ogre::MeshManager::getSingleton()
         .createPlane("ArenaPlane",
                      Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                      Ogre::Plane(Ogre::Vector3::UNIT_Y, 0),
                      mViewPort->getActualWidth(),
                      mViewPort->getActualHeight(),
                      20,
                      20,
                      true,
                      1,
                      5,
                      5,
                      Ogre::Vector3::UNIT_Z);

   auto entity = mArenaSceneMgr->createEntity("ArenaPlane");
   entity->setMaterialName("Examples/BeachStones");
   entity->setCastShadows(false);

   auto root_node = mArenaSceneMgr->getRootSceneNode();
   auto node = root_node->createChildSceneNode(node_name);
   node->attachObject(entity);
}

void Ogre3DRenderer::Render(const std::shared_ptr<Scoreboard>& scoreboard)
{
   SelectArenaScene();
}

void Ogre3DRenderer::Render(const std::shared_ptr<Wall>& wall)
{
   SelectArenaScene();
}

void Ogre3DRenderer::Render(const std::shared_ptr<Extra>& extra)
{
   SelectArenaScene();
}

void Ogre3DRenderer::Render(const std::shared_ptr<Bomb>& bomb)
{
   SelectArenaScene();
}

void Ogre3DRenderer::Render(const std::shared_ptr<Explosion>& explosion)
{
   SelectArenaScene();
}

void Ogre3DRenderer::Render(const std::shared_ptr<Player>& obj)
{
   SelectArenaScene();

   const auto type = obj->GetType();
   const auto parent_node_name = "Arena";
   std::string node_name;
   std::string mesh_name;

   if (PlayerType::Player_1 == type) {
      node_name = "Player_1";
      mesh_name = "cube.mesh";
   }
   else if (PlayerType::Player_2 == type) {
      node_name = "Player_2";
      mesh_name = "robot.mesh";
   }
   else if (PlayerType::Player_3 == type) {
      node_name = "Player_3";
      mesh_name = "athene.mesh";
   }
   else if (PlayerType::Player_4 == type) {
      node_name = "Player_4";
      mesh_name = "cube.mesh";
   }
   else {
      throw "Unknown player.";
   }

   Ogre::SceneNode* node = nullptr;

   if (mArenaSceneMgr->hasSceneNode(node_name))
   {
      node = mArenaSceneMgr->getSceneNode(node_name);
   }
   else
   {
      auto entity = mArenaSceneMgr->createEntity(mesh_name);
      auto parent_node = mArenaSceneMgr->getSceneNode(parent_node_name);

      node = parent_node->createChildSceneNode(node_name);
      node->attachObject(entity);

      const auto obj_size = obj->GetSize();
      const auto box_size = entity->getBoundingBox().getSize();

      const auto x_scale = obj_size.Width / box_size.x;
      const auto y_scale = obj_size.Height / box_size.y * 1.75f;
      const auto z_scale = obj_size.Width / box_size.z;
      node->setScale(x_scale, y_scale, z_scale);
      node->showBoundingBox(true);
   }

   const auto pos = obj->GetPosition();
   node->setPosition(pos.X - (mViewPort->getActualWidth() / 2),
                     40.0f,
                     pos.Y - (mViewPort->getActualHeight() / 2));

//   LOG(logDEBUG) << "Viewport: (" << mViewPort->getActualWidth() << ", "
//                 << mViewPort->getActualHeight() << ") - Player: ("
   //                 << pos.X << ", " << pos.Y << ")";

//   const auto world_v3 = node->convertLocalToWorldPosition(node->getPosition());
//   const auto local_v3 = node->convertWorldToLocalPosition({0, 40.0f, 0});
//   const auto sc = node->getScale();
//   const auto sc2 = node->getInheritScale();
//   int j = 0;
}

void Ogre3DRenderer::InitMenuScene()
{
   mMenuSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "MenuSceneMgr");
   mMenuSceneMgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

   auto cam = mMenuSceneMgr->createCamera("DefaultCamera");
   cam->setPosition(0.0f, 900.0f, 1.0f);
   cam->lookAt(0.0f, 0.0f, 0.0f);
   cam->setNearClipDistance(5.0f);

   auto light = mMenuSceneMgr->createLight("MenuSceneLight");
   light->setType(Ogre::Light::LT_POINT);
   light->setPosition(0.0f, 150.0f, 250.0f);
   light->setDiffuseColour(1.0f, 0.0f, 0.0f);
   light->setSpecularColour(1.0f, 0.0f, 0.0f);
}

void Ogre3DRenderer::InitArenaScene()
{
   mArenaSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "ArenaSceneMgr");
   mArenaSceneMgr->setAmbientLight(Ogre::ColourValue(1.7f, 1.7f, 1.7f));
   mArenaSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

   auto cam = mArenaSceneMgr->createCamera("DefaultCamera");
   cam->setPosition(0.0f, 900.0f, 600.0f);
   cam->lookAt(0.0f, 0.0f, 0.0f);
   cam->setNearClipDistance(5.0f);
}

void Ogre3DRenderer::SelectMenuScene()
{
   if (mMenuSceneSelected) {
      return;
   }

   SelectViewport(mMenuSceneMgr);
   mMenuSceneSelected = true;
}

void Ogre3DRenderer::SelectArenaScene()
{
   if (!mMenuSceneSelected) {
      return;
   }

   SelectViewport(mArenaSceneMgr);
   mMenuSceneSelected = false;
}

void Ogre3DRenderer::SelectViewport(Ogre::SceneManager* const scene_mgr)
{
   mRenderWindow->removeAllViewports();

   auto cam = scene_mgr->getCamera("DefaultCamera");
   mViewPort = mRenderWindow->addViewport(cam);
}
