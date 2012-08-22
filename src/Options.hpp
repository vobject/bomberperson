#ifndef DEFAULT_OPTIONS_HPP
#define DEFAULT_OPTIONS_HPP

#include "utils/Utils.hpp"

#include <SDL.h>

#include <string>

struct DefaultOptions
{
//public:
//   Options();
//   ~Options();

   static const std::string APP_NAME;
   static const int FPS_UPDATE_RATE = 1000_ms;

   static const int MAINLOOP_UPDATE_DELTA   = 2_ms;

   static const int SCREEN_WIDTH            = 640_px * 2;
   static const int SCREEN_HEIGHT           = 480_px * 2;

   static const std::string RESOURCE_DIR;

   static const int ARENA_BG_WIDTH  = SCREEN_WIDTH;
   static const int ARENA_BG_HEIGHT = SCREEN_HEIGHT;
   static const int ARENA_BG_BORDER_WIDTH  = 20_px * 2;
   static const int ARENA_BG_BORDER_HEIGHT = 16_px * 2;
   static const std::string ARENA_BG_ID;

   static const int ARENA_WIDTH             = ARENA_BG_WIDTH - (ARENA_BG_BORDER_WIDTH * 2);
   static const int ARENA_HEIGHT            = 432 * 2 - (ARENA_BG_BORDER_HEIGHT * 2);
   static const int ARENA_CELLS_X           = 15;
   static const int ARENA_CELLS_Y           = 11;
   static const std::string ARENA_ID;
   // TODO: Also add file names?

   static const int CELL_WIDTH              = ARENA_WIDTH / ARENA_CELLS_X;
   static const int CELL_HEIGHT             = ARENA_HEIGHT / ARENA_CELLS_Y;
   static const std::string CELL_ID;

   static const int WALL_WIDTH              = CELL_WIDTH;
   static const int WALL_HEIGHT             = CELL_HEIGHT;
   static const std::string WALL_ID_BRICKS;
   static const std::string WALL_ID_WOOD;

   static const int EXTRA_WIDTH             = CELL_WIDTH;
   static const int EXTRA_HEIGHT            = CELL_HEIGHT;
   static const std::string EXTRA_ID_SPEED;
   static const std::string EXTRA_ID_BOMBS;
   static const std::string EXTRA_ID_RANGE;

   static const int BOMB_WIDTH              = CELL_WIDTH;
   static const int BOMB_HEIGHT             = CELL_HEIGHT;
   static const std::string BOMB_ID;

   static const int EXPLOSION_WIDTH         = CELL_WIDTH;
   static const int EXPLOSION_HEIGHT        = CELL_HEIGHT;
   static const std::string EXPLOSION_ID;

   static const int PLAYER_WIDTH            = CELL_WIDTH * .7f;
   static const int PLAYER_HEIGHT           = CELL_HEIGHT * .7f;
   static const std::string PLAYER_ID_1;
   static const std::string PLAYER_ID_2;
   static const int PLAYER_1_CELL_X         = 0;
   static const int PLAYER_1_CELL_Y         = 0;
};

#endif // DEFAULT_OPTIONS_HPP
