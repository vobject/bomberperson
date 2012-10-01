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

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreViewport.h>
#include <OgreEntity.h>

#include <SDL.h>

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

   mResCache = make_unique<Ogre3DResourceCache>("Ogre3DRenderer", res);
   mRoot = Ogre::Root::getSingletonPtr();

   // Let OGRE blindly issue OpenGL calls because we already created
   //  a context to receive them.
   Ogre::NameValuePairList params = { { "currentGLContext", "True" } };
   auto wnd = mRoot->createRenderWindow("RenderWindow",
                                             res.Width,
                                             res.Height,
                                             false,
                                             &params);
   wnd->setVisible(true);

   // This can only be done after the render window was created.
   mResCache->LoadResources();

   mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

   auto cam = mSceneMgr->createCamera("DefaultCamera");
   cam->setPosition(0.0f, res.Width * 1.0f, 200.0f);
   cam->lookAt(0.0f, 0.0f, 0.0f);
   cam->setNearClipDistance(5.0f);
   mViewPort = wnd->addViewport(cam);
}

Ogre3DRenderer::~Ogre3DRenderer()
{

}

void Ogre3DRenderer::PreRender()
{
   mSceneMgr->clearScene();
//   mSceneMgr->setSkyDome(true, "Examples/CloudySky", 5, 8);
//   mSceneMgr->setAmbientLight(Ogre::ColourValue(1.7f, 1.7f, 1.7f));
   mSceneMgr->setSkyBox(true, "Examples/MorningSkyBox");
   mSceneMgr->setAmbientLight(Ogre::ColourValue(.5f, .5f, .5f));
   mSceneMgr->createLight()->setPosition(20, 80, 50);

   mRootNode = mSceneMgr->getRootSceneNode();
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
   const auto anim_time = obj->GetAnimationTime();

   auto entity = mSceneMgr->createEntity("ogrehead.mesh");

   auto node = mRootNode->createChildSceneNode();
   node->attachObject(entity);
   node->pitch(Ogre::Degree(-90.0f));
   node->scale(.7f, .7f, .7f);

   const auto pos = obj->GetPosition();
   node->setPosition(pos.X - (mViewPort->getActualWidth() / 2),
                     40.0f,
                     pos.Y - (mViewPort->getActualHeight() / 2));
   node->yaw(Ogre::Degree(anim_time % 360));
}

void Ogre3DRenderer::Render(const std::shared_ptr<MainMenu>& obj)
{
   const auto node_name = "MainMenuBackground";

   auto entity = mSceneMgr->createEntity("MainMenuBgPlane");
   entity->setMaterialName("Examples/Rockwall");

   auto node = mRootNode->createChildSceneNode(node_name);
   node->attachObject(entity);
}

void Ogre3DRenderer::Render(const std::shared_ptr<Arena>& obj)
{
   (void) obj;

   const auto mesh_name = mResCache->GetArenaResource();
   auto entity = mSceneMgr->createEntity(mesh_name);
   entity->setMaterialName("Examples/BumpyMetal");

   auto node = mRootNode->createChildSceneNode();
   node->attachObject(entity);
}

void Ogre3DRenderer::Render(const std::shared_ptr<Scoreboard>& obj)
{
   (void) obj;
}

void Ogre3DRenderer::Render(const std::shared_ptr<Wall>& obj)
{
   const auto mesh_name = mResCache->GetWallResource(obj->GetType());
   auto entity = mSceneMgr->createEntity(mesh_name);

   if (WallType::Indestructible == obj->GetType()) {
      entity->setMaterialName("2 - Default");
   }
   else if (WallType::Destructible == obj->GetType()) {
      entity->setMaterialName("Examples/Rockwall");
   }

   auto node = AddSceneNode(*obj, entity);
//   node->showBoundingBox(true);
//   node->setVisible(false);
}

void Ogre3DRenderer::Render(const std::shared_ptr<Extra>& obj)
{
   const auto mesh_name = mResCache->GetExtraResource(obj->GetType());
   auto entity = mSceneMgr->createEntity(mesh_name);

   auto node = AddSceneNode(*obj, entity);
   node->showBoundingBox(true);
}

void Ogre3DRenderer::Render(const std::shared_ptr<Bomb>& obj)
{
   const auto mesh_name = mResCache->GetBombResource(obj->GetType());
   auto entity = mSceneMgr->createEntity(mesh_name);

   auto node = AddSceneNode(*obj, entity);
//   node->showBoundingBox(true);
}

void Ogre3DRenderer::Render(const std::shared_ptr<Explosion>& obj)
{
   const auto mesh_name = mResCache->GetExplosionResource(obj->GetType());
   auto entity = mSceneMgr->createEntity(mesh_name);
   entity->setMaterialName("Examples/Hilite/Yellow");

   auto node = AddSceneNode(*obj, entity);
//   node->showBoundingBox(true);
}

void Ogre3DRenderer::Render(const std::shared_ptr<Player>& obj)
{
   const auto mesh_name = mResCache->GetPlayerResource(obj->GetType());
   auto entity = mSceneMgr->createEntity(mesh_name);

   auto node = AddSceneNode(*obj, entity);
//   node->showBoundingBox(true);

//   LOG(logDEBUG) << "Viewport: (" << mViewPort->getActualWidth() << ", "
//                 << mViewPort->getActualHeight() << ") - Player: ("
//                 << pos.X << ", " << pos.Y << ")";

//   const auto world_v3 = node->convertLocalToWorldPosition(node->getPosition());
//   const auto local_v3 = node->convertWorldToLocalPosition({0, 40.0f, 0});
//   const auto sc = node->getScale();
//   const auto sc2 = node->getInheritScale();
}

Ogre::SceneNode *Ogre3DRenderer::AddSceneNode(
   const SceneObject& obj,
   Ogre::Entity* const entity
)
{
   auto node = mRootNode->createChildSceneNode();
   node->attachObject(entity);

   const auto obj_size = obj.GetSize();
   const auto box_size = entity->getBoundingBox().getSize();

   const auto x_scale = obj_size.Width / box_size.x;
   const auto y_scale = obj_size.Height / box_size.y;
   const auto z_scale = obj_size.Height / box_size.z;
   node->setScale(x_scale, y_scale, z_scale);

   const auto pos = obj.GetPosition();
   const auto x_delta = mViewPort->getActualWidth() / 2;
   const auto z_delta = mViewPort->getActualHeight() / 2;
   node->setPosition(pos.X - x_delta,
                     40.0f,
                     pos.Y - z_delta);
   return node;
}

//void Ogre3DRenderer::InitMenuScene(Ogre::SceneManager* const scene_mgr)
//{
//   scene_mgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "MenuSceneMgr");
//   scene_mgr->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));

//   auto cam = scene_mgr->createCamera("DefaultCamera");
//   cam->setPosition(0.0f, 900.0f, 1.0f);
//   cam->lookAt(0.0f, 0.0f, 0.0f);
//   cam->setNearClipDistance(5.0f);
//   mViewPort = mRenderWindow->addViewport(cam);

//   auto light = scene_mgr->createLight("MenuSceneLight");
//   light->setType(Ogre::Light::LT_POINT);
//   light->setPosition(0.0f, 150.0f, 250.0f);
//   light->setDiffuseColour(1.0f, 0.0f, 0.0f);
//   light->setSpecularColour(1.0f, 0.0f, 0.0f);
//}

//void Ogre3DRenderer::InitArenaScene(Ogre::SceneManager* const scene_mgr)
//{
//   scene_mgr = mRoot->createSceneManager(Ogre::ST_GENERIC, "ArenaSceneMgr");
//   scene_mgr->setAmbientLight(Ogre::ColourValue(1.7f, 1.7f, 1.7f));
//   scene_mgr->setSkyDome(true, "Examples/CloudySky", 5, 8);

//   auto cam = scene_mgr->createCamera("DefaultCamera");
//   cam->setPosition(0.0f, 900.0f, 500.0f);
//   cam->lookAt(0.0f, 0.0f, 0.0f);
//   cam->setNearClipDistance(5.0f);
//   mViewPort = mRenderWindow->addViewport(cam);
//}
