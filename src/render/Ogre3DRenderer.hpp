#ifndef OGRE3D_RENDERER_HPP
#define OGRE3D_RENDERER_HPP

#include "Renderer.hpp"

#include <Ogre.h>

struct Size;

class Ogre3DRenderer : public Renderer
{
public:
   Ogre3DRenderer(Size res);
   virtual ~Ogre3DRenderer();

   Ogre3DRenderer(const Ogre3DRenderer&) = delete;
   Ogre3DRenderer& operator=(const Ogre3DRenderer&) = delete;

   void PreRender() override;
   void PostRender() override;

   void Render(const std::shared_ptr<MenuItem>& obj) override;
   void Render(const std::shared_ptr<MenuItemSelector>& obj) override;
   void Render(const std::shared_ptr<MainMenu>& obj) override;

   void Render(const std::shared_ptr<Arena>& arena) override;
   void Render(const std::shared_ptr<Scoreboard>& scoreboard) override;
   void Render(const std::shared_ptr<Wall>& explosion) override;
   void Render(const std::shared_ptr<Extra>& bomb) override;
   void Render(const std::shared_ptr<Bomb>& bomb) override;
   void Render(const std::shared_ptr<Explosion>& explosion) override;
   void Render(const std::shared_ptr<Player>& player) override;

private:
   Ogre::Root* mRoot = nullptr;
   Ogre::SceneManager* mSceneMgr = nullptr;
   Ogre::Camera* mCamera = nullptr;
   Ogre::Viewport* mViewPort = nullptr;
   Ogre::RenderWindow* mRenderWindow = nullptr;

   // Resources
   Ogre::SceneNode* mMainMenuBgNode = nullptr;
   Ogre::SceneNode* mHeadNode1 = nullptr;
   Ogre::SceneNode* mHeadNode2 = nullptr;
};

#endif // OGRE3D_RENDERER_HPP
