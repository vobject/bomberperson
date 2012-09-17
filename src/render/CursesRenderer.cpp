#include "CursesRenderer.hpp"
#include "../game/UserInterface.hpp"
#include "../game/MainMenu.hpp"
#include "../game/Arena.hpp"
#include "../game/Scoreboard.hpp"
#include "../game/Wall.hpp"
#include "../game/Extra.hpp"
#include "../game/Bomb.hpp"
#include "../game/Explosion.hpp"
#include "../game/Player.hpp"
#include "../utils/Utils.hpp"

#include <curses.h>

CursesRenderer::CursesRenderer(const Size res)
{
   initscr();

   int x_res;
   int y_res;
   getyx(stdscr, y_res, x_res);

   mXResFactor = x_res / res.Width;
   mYResFactor = y_res / res.Height;

   curs_set(0);
}

CursesRenderer::~CursesRenderer()
{
   endwin();
}

void CursesRenderer::PreRender()
{
//   clear();

//   mvprintw(3, 5, "LINES: %d", LINES);
//   mvprintw(4, 5, "COLS:  %d", COLS);

//   mvprintw(5, 5, "Momentane Cursorposition:  [%d, %d]", y, x);

//   getbegyx(stdscr, y, x);
//   mvprintw(6, 5, "Koordinatenursprung:       [%d, %d]", y, x);

//   getmaxyx(stdscr, y, x);
//   mvprintw(7, 5, "Fenstergröße:              [%d, %d]", y, x);

//   mvaddstr(11, 2, "Taste drücken -> Ende");
}

void CursesRenderer::PostRender()
{
   refresh();
}

void CursesRenderer::Render(const std::shared_ptr<MainMenu>& mainmenu)
{
   const auto pos = mainmenu->GetPosition();
   const auto size = mainmenu->GetSize();

//   box(stdscr, '|', '*');
   move(10, 30);
   addch('*');
}

void CursesRenderer::Render(const std::shared_ptr<Arena>& arena)
{

}

void CursesRenderer::Render(const std::shared_ptr<Scoreboard>& scoreboard)
{

}

void CursesRenderer::Render(const std::shared_ptr<Wall>& wall)
{

}

void CursesRenderer::Render(const std::shared_ptr<Extra>& extra)
{

}

void CursesRenderer::Render(const std::shared_ptr<Bomb>& bomb)
{

}

void CursesRenderer::Render(const std::shared_ptr<Explosion>& explosion)
{

}

void CursesRenderer::Render(const std::shared_ptr<Player>& player)
{

}
