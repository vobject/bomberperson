#include "Scoreboard.hpp"
#include "Arena.hpp"
#include "Player.hpp"

#include <sstream>

Scoreboard::Scoreboard()
   : SceneObject(EntityId::Scoreboard)
{
   SetZOrder(ZOrder::Layer_1);
}

Scoreboard::~Scoreboard()
{

}

void Scoreboard::Update(const int elapsed_time)
{

}

void Scoreboard::KeepTrackOf(const std::shared_ptr<Arena> &arena)
{
   mArena = arena;
}

void Scoreboard::KeepTrackOf(const std::vector<std::shared_ptr<Player>>& players)
{
   mPlayers = players;
}

std::vector<std::string> Scoreboard::GetScore() const
{
   std::vector<std::string> lines;
   std::ostringstream os;

   lines.push_back("Arena:");

   os << "  Cells: " << mArena->GetCells().size();
   lines.push_back(os.str());

//   os.clear();
//   os << "\tCell width: " << mArena->GetCellSize().Width << "px";
//   lines.push_back(os.str());

//   os.clear();
//   os << "\tCell height: " << mArena->GetCellSize().Height << "px";
//   lines.push_back(os.str());

   for (const auto& player : mPlayers)
   {
//      lines.push_back("");

//      os.clear();
//      os.str("");
//      os << "Player " << static_cast<int>(player->GetId()) << ":";
//      lines.push_back(os.str());

//      os.clear();
//      os.str("");
//      os << "  Speed: " << player->GetSpeed();
//      lines.push_back(os.str());
   }

   return lines;
}
