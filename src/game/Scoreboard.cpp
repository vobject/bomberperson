#include "Scoreboard.hpp"
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
   mGameTime += elapsed_time;
}

void Scoreboard::KeepTrackOf(const std::shared_ptr<Player>& player)
{
   mPlayers.push_back(player);
}

std::vector<std::string> Scoreboard::GetScore() const
{
   std::vector<std::string> lines;
   std::ostringstream os;

   os << "Time: " << (mGameTime / 1000) << "s";
   lines.push_back(os.str());
   os.clear();
   os.str("");

   for (const auto& player : mPlayers)
   {
      lines.push_back("");

      const auto data = player->GetData();

      os << "Player " << static_cast<int>(player->GetId()) << ":";
      lines.push_back(os.str());
      os.clear();
      os.str("");

      os << "  Speed: " << data.speed;
      lines.push_back(os.str());
      os.clear();
      os.str("");

      os << "  Bombs: " << data.bombs;
      lines.push_back(os.str());
      os.clear();
      os.str("");

      os << "  Range: " << data.range;
      lines.push_back(os.str());
      os.clear();
      os.str("");

      os << "  Wins: " << data.wins;
      lines.push_back(os.str());
      os.clear();
      os.str("");
   }

   return lines;
}
