#include "SimpleGlRenderer.hpp"
#include "../../game/UserInterface.hpp"
#include "../../game/MenuItem.hpp"
#include "../../game/MenuItemSelector.hpp"
#include "../../game/MainMenu.hpp"
#include "../../game/Arena.hpp"
#include "../../game/Scoreboard.hpp"
#include "../../game/Wall.hpp"
#include "../../game/Extra.hpp"
#include "../../game/Bomb.hpp"
#include "../../game/Explosion.hpp"
#include "../../game/Player.hpp"
#include "../../utils/Utils.hpp"

#include <SDL.h>

#include <GL/gl.h>
#include <GL/glu.h>

SimpleGlRenderer::SimpleGlRenderer(const Size res)
{
   if (0 > SDL_Init(SDL_INIT_VIDEO)) {
      throw "Cannot init SDL video subsystem.";
   }
   atexit(SDL_Quit);

   SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
   const auto screen = SDL_SetVideoMode(res.Width,
                                        res.Height,
                                        32,
                                        SDL_OPENGL);
   if (!screen) {
      throw "SDL_SetVideoMode() failed.";
   }

   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

   glEnable(GL_BLEND);
   glEnable(GL_LINE_SMOOTH);
}

SimpleGlRenderer::~SimpleGlRenderer()
{

}

void SimpleGlRenderer::PreRender()
{
   // Screen size might have changed.
   mScreen = SDL_GetVideoSurface();

   glViewport(0, 0, mScreen->w, mScreen->h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, mScreen->w, mScreen->h, 0.0, -1.0, 1.0);

   glClear(GL_COLOR_BUFFER_BIT);
}

void SimpleGlRenderer::PostRender()
{
   SDL_GL_SwapBuffers();
}

void SimpleGlRenderer::Render(const std::shared_ptr<MenuItem>& obj)
{
   // TODO: Implement Me!
}

void SimpleGlRenderer::Render(const std::shared_ptr<MenuItemSelector>& obj)
{
   // TODO: Implement Me!
}

void SimpleGlRenderer::Render(const std::shared_ptr<MainMenu>& obj)
{
   // TODO: Implement Me!
}

void SimpleGlRenderer::Render(const std::shared_ptr<Arena>& arena)
{
   // TODO: Implement Me!
}

void SimpleGlRenderer::Render(const std::shared_ptr<Scoreboard>& scoreboard)
{
   // TODO: Implement Me!
}

void SimpleGlRenderer::Render(const std::shared_ptr<Wall>& wall)
{
   // TODO: Implement Me!
}

void SimpleGlRenderer::Render(const std::shared_ptr<Extra>& extra)
{
   // TODO: Implement Me!
}

void SimpleGlRenderer::Render(const std::shared_ptr<Bomb>& bomb)
{
   // TODO: Implement Me!
}

void SimpleGlRenderer::Render(const std::shared_ptr<Explosion>& explosion)
{
   // TODO: Implement Me!
}

void SimpleGlRenderer::Render(const std::shared_ptr<Player>& player)
{
   // TODO: Implement Me!
}
