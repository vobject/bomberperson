#ifndef DEFAULT_OPTIONS_HPP
#define DEFAULT_OPTIONS_HPP

#include "utils/Utils.hpp"

#include <SDL.h>

#include <string>

static const std::string RESOURCE_DIR = "res_q1";

struct DefaultValue
{
   static const int SCREEN_WIDTH            = 800_px;
   static const int SCREEN_HEIGHT           = 600_px;

   static const int ARENA_POS_X             = 0_px;
   static const int ARENA_POS_Y             = 0_px;
   static const int ARENA_WIDTH             = SCREEN_WIDTH * .8;
   static const int ARENA_HEIGHT            = SCREEN_HEIGHT;
   static const int ARENA_BORDER_WIDTH      = .03125 * SCREEN_WIDTH;
   static const int ARENA_BORDER_HEIGHT     = .03704 * SCREEN_HEIGHT;
   static const int ARENA_CELLS_X           = 15;
   static const int ARENA_CELLS_Y           = 11;

   static const int SCOREBOARD_POS_X        = ARENA_POS_X + ARENA_WIDTH;
   static const int SCOREBOARD_POS_Y        = ARENA_POS_Y;
   static const int SCOREBOARD_WIDTH        = SCREEN_WIDTH - ARENA_WIDTH;
   static const int SCOREBOARD_HEIGHT       = ARENA_HEIGHT;

   static const int CELL_WIDTH              = (ARENA_WIDTH - ARENA_BORDER_WIDTH) / ARENA_CELLS_X;
   static const int CELL_HEIGHT             = (ARENA_HEIGHT - ARENA_BORDER_HEIGHT) / ARENA_CELLS_Y;

   static const int WALL_WIDTH              = CELL_WIDTH;
   static const int WALL_HEIGHT             = CELL_HEIGHT;

   static const int EXTRA_WIDTH             = CELL_WIDTH;
   static const int EXTRA_HEIGHT            = CELL_HEIGHT;

   static const int BOMB_WIDTH              = CELL_WIDTH;
   static const int BOMB_HEIGHT             = CELL_HEIGHT;
   static const int BOMB_ANIM_LEN           = 2500_ms;

   static const int EXPLOSION_WIDTH         = CELL_WIDTH;
   static const int EXPLOSION_HEIGHT        = CELL_HEIGHT;
   static const int EXPLOSION_ANIM_LEN      = 1000_ms;

   static const int PLAYER_WIDTH            = CELL_WIDTH * 1.75f;
   static const int PLAYER_HEIGHT           = CELL_HEIGHT * 1.75f;
   static const int PLAYER_DEATH_ANIM_LEN   = 1000_ms;
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
