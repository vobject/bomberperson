#ifndef OGRE3D_RENDERER_HPP
#define OGRE3D_RENDERER_HPP

#include "Renderer.hpp"

namespace Ogre {
   class Root;
   class SceneManager;
   class Viewport;
   class SceneNode;
   class Entity;
}

struct Size;
class Ogre3DResourceCache;

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

   void Render(const std::shared_ptr<Arena>& obj) override;
   void Render(const std::shared_ptr<Scoreboard>& obj) override;
   void Render(const std::shared_ptr<Wall>& obj) override;
   void Render(const std::shared_ptr<Extra>& obj) override;
   void Render(const std::shared_ptr<Bomb>& obj) override;
   void Render(const std::shared_ptr<Explosion>& obj) override;
   void Render(const std::shared_ptr<Player>& obj) override;

private:
   Ogre::SceneNode* AddSceneNode(const SceneObject& obj, Ogre::Entity* const entity);

   std::unique_ptr<Ogre3DResourceCache> mResCache;

   // None of these pointers are owning.
   Ogre::Root* mRoot = nullptr;
   Ogre::SceneManager* mSceneMgr = nullptr;
   Ogre::Viewport* mViewPort = nullptr;

   Ogre::SceneNode* mRootNode = nullptr;
};

#endif // OGRE3D_RENDERER_HPP
