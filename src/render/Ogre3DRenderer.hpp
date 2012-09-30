#ifndef OGRE3D_RENDERER_HPP
#define OGRE3D_RENDERER_HPP

#include "Renderer.hpp"

namespace Ogre {
   class Root;
   class RenderWindow;
   class SceneManager;
   class Viewport;
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

   void AddObject(const std::shared_ptr<Arena>& obj) override;
   void AddObject(const std::shared_ptr<Scoreboard>& obj) override;
   void AddObject(const std::shared_ptr<Wall>& obj) override;
   void AddObject(const std::shared_ptr<Extra>& obj) override;
   void AddObject(const std::shared_ptr<Bomb>& obj) override;
   void AddObject(const std::shared_ptr<Explosion>& obj) override;
   void AddObject(const std::shared_ptr<Player>& obj) override;

   void RemoveObject(unsigned int instance) override;
   void Clear() override;

   void PreRender() override;
   void PostRender() override;
   void Render() override;

//   void Render(const std::shared_ptr<MenuItem>& obj) override;
//   void Render(const std::shared_ptr<MenuItemSelector>& obj) override;
//   void Render(const std::shared_ptr<MainMenu>& obj) override;

//   void Render(const std::shared_ptr<Arena>& arena) override;
//   void Render(const std::shared_ptr<Scoreboard>& scoreboard) override;
//   void Render(const std::shared_ptr<Wall>& explosion) override;
//   void Render(const std::shared_ptr<Extra>& obj) override;
//   void Render(const std::shared_ptr<Bomb>& bomb) override;
//   void Render(const std::shared_ptr<Explosion>& explosion) override;
//   void Render(const std::shared_ptr<Player>& obj) override;

private:
//   void InitMenuScene();
   void InitArenaScene();

//   void SelectMenuScene();
   void SelectArenaScene();
   void SelectViewport(Ogre::SceneManager* const scene_mgr);

   std::string InstanceIdToNodeName(unsigned int instance) const;

   std::unique_ptr<Ogre3DResourceCache> mResCache;

   // None of these pointers are owning.
   Ogre::Root* mRoot = nullptr;
   Ogre::RenderWindow* mRenderWindow = nullptr;
//   Ogre::SceneManager* mMenuSceneMgr = nullptr;
   Ogre::SceneManager* mArenaSceneMgr = nullptr;
   Ogre::Viewport* mViewPort = nullptr;

//   bool mMenuSceneSelected = true;
};

#endif // OGRE3D_RENDERER_HPP
