#ifndef DEFAULT_OPTIONS_HPP
#define DEFAULT_OPTIONS_HPP

#include "utils/Utils.hpp"

#include <SDL.h>

#include <string>

struct DefaultSize
{
   static const int SCREEN_WIDTH            = 640_px;
   static const int SCREEN_HEIGHT           = 480_px;

   static const int ARENA_BG_WIDTH          = SCREEN_WIDTH;
   static const int ARENA_BG_HEIGHT         = SCREEN_HEIGHT;

   static const int ARENA_POS_X             = 20_px;
   static const int ARENA_POS_Y             = 16_px;
   static const int ARENA_WIDTH             = SCREEN_WIDTH - (ARENA_POS_X * 2);
   static const int ARENA_HEIGHT            = SCREEN_HEIGHT - (ARENA_POS_Y * 2) - 48;
   static const int ARENA_CELLS_X           = 15;
   static const int ARENA_CELLS_Y           = 11;

   static const int CELL_WIDTH              = ARENA_WIDTH / ARENA_CELLS_X;
   static const int CELL_HEIGHT             = ARENA_HEIGHT / ARENA_CELLS_Y;

   static const int WALL_WIDTH              = CELL_WIDTH;
   static const int WALL_HEIGHT             = CELL_HEIGHT;

   static const int EXTRA_WIDTH             = CELL_WIDTH;
   static const int EXTRA_HEIGHT            = CELL_HEIGHT;

   static const int BOMB_WIDTH              = CELL_WIDTH;
   static const int BOMB_HEIGHT             = CELL_HEIGHT;

   static const int EXPLOSION_WIDTH         = CELL_WIDTH;
   static const int EXPLOSION_HEIGHT        = CELL_HEIGHT;

   static const int PLAYER_WIDTH            = CELL_WIDTH * .7f;
   static const int PLAYER_HEIGHT           = CELL_HEIGHT * .7f;
   static const int PLAYER_1_CELL_X         = 0;
   static const int PLAYER_1_CELL_Y         = 0;
   static const int PLAYER_2_CELL_X         = 14;
   static const int PLAYER_2_CELL_Y         = 10;
   static const int PLAYER_3_CELL_X         = 0;
   static const int PLAYER_3_CELL_Y         = 10;
   static const int PLAYER_4_CELL_X         = 14;
   static const int PLAYER_4_CELL_Y         = 0;
};

#endif // DEFAULT_OPTIONS_HPP
