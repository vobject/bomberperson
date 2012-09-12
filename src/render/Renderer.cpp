#include "Renderer.hpp"
#include "../game/MainMenu.hpp"
#include "../game/Arena.hpp"
#include "../game/Scoreboard.hpp"
#include "../game/Wall.hpp"
#include "../game/Extra.hpp"
#include "../game/Bomb.hpp"
#include "../game/Explosion.hpp"
#include "../game/Player.hpp"
#include "../utils/Utils.hpp"

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

}

void Renderer::PreRender()
{

}

void Renderer::PostRender()
{

}

void Renderer::Render(const std::shared_ptr<SceneObject>& obj)
{
   // Quick & dirty hack to support polymorthism by function parameter.
   // Stupid C++ does not support multiple dispatch natively and I am not
   //  motivated to implement the visitor pattern or double dispatch.
   // So we'll be using good old dynamic_cast (sort of).

   if (const auto ptr = std::dynamic_pointer_cast<MainMenu>(obj)) {
      Render(ptr);
   }
   else if (const auto ptr = std::dynamic_pointer_cast<Arena>(obj)) {
      Render(ptr);
   }
   else if (const auto ptr = std::dynamic_pointer_cast<Scoreboard>(obj)) {
      Render(ptr);
   }
   else if (const auto ptr = std::dynamic_pointer_cast<Wall>(obj)) {
      Render(ptr);
   }
   else if (const auto ptr = std::dynamic_pointer_cast<Bomb>(obj)) {
      Render(ptr);
   }
   else if (const auto ptr = std::dynamic_pointer_cast<Explosion>(obj)) {
      Render(ptr);
   }
   else if (const auto ptr = std::dynamic_pointer_cast<Extra>(obj)) {
      Render(ptr);
   }
   else if (const auto ptr = std::dynamic_pointer_cast<Player>(obj)) {
      Render(ptr);
   }
   else {
      LOG(logERROR) << "SdlRenderer::Render(SceneObject) Unknown object!";
   }
}
