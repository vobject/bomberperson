#include "RaycastRenderer.hpp"
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
#include "../../Options.hpp"

#include <SDL.h>


// Initial raycasting code based on http://lodev.org/cgtutor/raycasting.html

#include <cmath>

//std::vector<std::vector<int>> worldMap =
//{
//  {1,2,2,1,1,1,1,1,1,3,1},
//  {3,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,4},
//  {4,0,0,0,0,0,0,0,0,0,3},
//  {1,0,0,0,0,0,3,2,2,2,1},
//  {1,0,0,0,0,0,2,0,0,0,1},
//  {1,0,0,0,0,0,2,0,0,0,1},
//  {1,0,0,0,0,0,2,0,0,0,1},
//  {1,0,0,0,0,0,2,2,0,2,1},
//  {1,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,1},
//  {1,0,0,0,0,0,0,0,0,0,1},
//  {3,0,0,0,0,0,0,0,0,0,3},
//  {1,0,1,1,1,1,1,1,1,3,1}
//};

//x and y start position
double posX = 3;
double posY = 3;

//initial direction vector
double dirX = 1;
double dirY = 0;

//the 2d raycaster version of camera plane
double planeX = 0;
double planeY = 0.66;



RaycastRenderer::RaycastRenderer(const Size res)
   : mResolution(res)
   , mMap(DefaultValue::ARENA_CELLS_X, std::vector<int>(DefaultValue::ARENA_CELLS_Y, 0))
{
   if (0 > SDL_Init(SDL_INIT_VIDEO)) {
      throw "Cannot init SDL video subsystem.";
   }
   atexit(SDL_Quit);

   const auto screen = SDL_SetVideoMode(mResolution.Width,
                                        mResolution.Height,
                                        32,
                                        SDL_ANYFORMAT |
                                           SDL_SWSURFACE |
                                           SDL_DOUBLEBUF);
   if (!screen) {
      throw "SDL_SetVideoMode() failed.";
   }

   // The return value of SDL_SetVideoMode() (-> screen) should not be freed
   //  by the caller. The man page tells us to rely on SDL_Quit() to do this.
}

RaycastRenderer::~RaycastRenderer()
{

}

void RaycastRenderer::PreRender()
{
   // Screen size might have changed.
   mScreen = SDL_GetVideoSurface();

   const auto ceiling = SDL_MapRGB(mScreen->format, 0x60, 0x60, 0x60);
   SDL_Rect ceiling_rect = { static_cast<Sint16>(0),
                             static_cast<Sint16>(0),
                             static_cast<Uint16>(mResolution.Width),
                             static_cast<Uint16>(mResolution.Height / 2) };
   SDL_FillRect(mScreen, &ceiling_rect, ceiling);

   const auto floor = SDL_MapRGB(mScreen->format, 0x80, 0x80, 0x80);
   SDL_Rect floor_rect = { static_cast<Sint16>(0),
                           static_cast<Sint16>(mResolution.Height / 2),
                           static_cast<Uint16>(mResolution.Width),
                           static_cast<Uint16>(mResolution.Height / 2) };
   SDL_FillRect(mScreen, &floor_rect, floor);

   for (auto& row : mMap)
   {
      for (auto& elem : row)
      {
         elem = 0;
      }
   }
}

void RaycastRenderer::PostRender()
{
   for(int x = 0; x < mResolution.Width; x++)
   {
      //calculate ray position and direction
      double cameraX = 2 * x / double(mResolution.Width) - 1; //x-coordinate in camera space
      double rayPosX = posX + 2;
      double rayPosY = posY + 2;
      double rayDirX = dirX + planeX * cameraX;
      double rayDirY = dirY + planeY * cameraX;

      //which box of the map we're in
      int mapX = rayPosX;
      int mapY = rayPosY;

      //length of ray from current position to next x or y-side
      double sideDistX;
      double sideDistY;

      //length of ray from one x or y-side to next x or y-side
      double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
      double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
      double perpWallDist;

      //what direction to step in x or y-direction (either +1 or -1)
      int stepX;
      int stepY;

      int hit = 0; //was there a wall hit?
      int side; //was a NS or a EW wall hit?

      //calculate step and initial sideDist
      if (rayDirX < 0)
      {
         stepX = -1;
         sideDistX = (rayPosX - mapX) * deltaDistX;
      }
      else
      {
         stepX = 1;
         sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
      }
      if (rayDirY < 0)
      {
         stepY = -1;
         sideDistY = (rayPosY - mapY) * deltaDistY;
      }
      else
      {
         stepY = 1;
         sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
      }

      //perform DDA
      while (hit == 0)
      {
         //jump to next map square, OR in x-direction, OR in y-direction
         if (sideDistX < sideDistY)
         {
            sideDistX += deltaDistX;
            mapX += stepX;
            side = 0;
         }
         else
         {
            sideDistY += deltaDistY;
            mapY += stepY;
            side = 1;
         }

         //Check if ray has hit a wall
         if ((mapX >= mMap.size()) || (mapY >= mMap[0].size()))
         {
            hit = 1;
            break;
         }

         if (mMap[mapX][mapY] > 0)
         {
            hit = 1;
            break;
         }
      }

      //Calculate distance projected on camera direction (oblique distance will give fisheye effect!)
      if (side == 0)
         perpWallDist = fabs((mapX - rayPosX + (1 - stepX) / 2) / rayDirX);
      else
         perpWallDist = fabs((mapY - rayPosY + (1 - stepY) / 2) / rayDirY);

      //Calculate height of line to draw on screen
      int lineHeight = abs(int(mResolution.Height / perpWallDist));

      //calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + mResolution.Height / 2;
      if(drawStart < 0) drawStart = 0;
      int drawEnd = lineHeight / 2 + mResolution.Height / 2;
      if(drawEnd >= mResolution.Height) drawEnd = mResolution.Height - 1;

      //choose wall color
      SDL_Color color;
      if ((mapX >= mMap.size()) || (mapY >= mMap[0].size()))
      {
         color = { 0xff, 0x00, 0x00 }; // red borders
      }
      else
      {
         switch(mMap[mapX][mapY])
         {
            case 1:  color = { 0xff, 0x00, 0x00 }; break; //red
            case 2:  color = { 0x00, 0xff, 0x00 }; break; //green
            case 3:  color = { 0x00, 0x00, 0xff }; break; //blue
            case 4:  color = { 0xff, 0xff, 0xff }; break; //white
            case 5:  color = { 0xff, 0xff, 0x00 }; break; //yellow
            default: color = { 0x30, 0x30, 0x30 }; break; //dark gray
         }
      }

      //give x and y sides different brightness
      if (side == 1)
      {
         color.r = color.r * .7f;
         color.g = color.g * .7f;
         color.b = color.b * .7f;
      }

      //draw the pixels of the stripe as a vertical line
      DrawVerticalLine(x, drawStart, drawEnd, color);
   }

   double rotSpeed = .0035; //the constant value is in radians/second

   //both camera direction and camera plane must be rotated
   double oldDirX = dirX;
   dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
   dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
   double oldPlaneX = planeX;
   planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
   planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);

   SDL_Flip(mScreen);
}

void RaycastRenderer::Render(const std::shared_ptr<MenuItem>& obj)
{

}

void RaycastRenderer::Render(const std::shared_ptr<MenuItemSelector>& obj)
{

}

void RaycastRenderer::Render(const std::shared_ptr<MainMenu>& obj)
{

}

void RaycastRenderer::Render(const std::shared_ptr<Arena>& arena)
{
   mArena = arena.get();
}

void RaycastRenderer::Render(const std::shared_ptr<Scoreboard>& scoreboard)
{

}

void RaycastRenderer::Render(const std::shared_ptr<Wall>& wall)
{
   const auto cell = mArena->GetCellFromObject(*wall);

   if (WallType::Indestructible == wall->GetType())
   {
      mMap[cell.X][cell.Y] = 99;
   }
   else if (WallType::Destructible == wall->GetType())
   {
//      mMap[cell.X][cell.Y] = 5;
   }
}

void RaycastRenderer::Render(const std::shared_ptr<Extra>& extra)
{

}

void RaycastRenderer::Render(const std::shared_ptr<Bomb>& bomb)
{

}

void RaycastRenderer::Render(const std::shared_ptr<Explosion>& explosion)
{

}

void RaycastRenderer::Render(const std::shared_ptr<Player>& player)
{
   const auto cell = mArena->GetCellFromObject(*player);
   posX = cell.X;
   posY = cell.Y;
}

void RaycastRenderer::DrawVerticalLine(
   const int x,
   int y1,
   int y2,
   const SDL_Color color
)
{
   if(y2 < y1)
   {
      //swap y1 and y2
      y1 += y2;
      y2 = y1 - y2;
      y1 -= y2;
   }

   if(y2 < 0 || y1 >= mResolution.Height  || x < 0 || x >= mResolution.Width)
   {
      return; //no single point of the line is on screen
   }

   if(y1 < 0)
   {
      y1 = 0; //clip
   }

   if(y2 >= mResolution.Width)
   {
      y2 = mResolution.Height - 1; //clip
   }

   Uint32 colorSDL = SDL_MapRGB(mScreen->format, color.r, color.g, color.b);
   Uint32* bufp;

   bufp = (Uint32*)mScreen->pixels + y1 * mScreen->pitch / 4 + x;

   for(int y = y1; y <= y2; y++)
   {
      *bufp = colorSDL;
      bufp += mScreen->pitch / 4;
   }
}
