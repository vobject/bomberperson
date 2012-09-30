#include "Scoreboard.hpp"
#include "EventQueue.hpp"
#include "EventType.hpp"
#include "Player.hpp"

#include <sstream>

Scoreboard::Scoreboard(EventQueue& queue)
   : SceneObject(EntityId::Scoreboard, ZOrder::Layer_1)
   , mEventQueue(queue)
{
   mEventQueue.Register(this);
}

Scoreboard::~Scoreboard()
{
   mEventQueue.UnRegister(this);
}

void Scoreboard::Update(const int elapsed_time)
{
   mGameTime += elapsed_time;
}

void Scoreboard::OnEvent(const Event& event)
{
   switch (event.GetType())
   {
      case EventType::RemoveScoreboard:
         OnRemoveScoreboard(dynamic_cast<const RemoveScoreboardEvent&>(event));
         break;
      case EventType::CreatePlayer:
         OnCreatePlayer(dynamic_cast<const CreatePlayerEvent&>(event));
         break;
      case EventType::KillPlayer:
         OnKillPlayer(dynamic_cast<const KillPlayerEvent&>(event));
         break;
      default:
         break;
   }
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

      os << "Player " << static_cast<int>(player.first) + 1 << ":";
      lines.push_back(os.str());
      os.clear();
      os.str("");

      os << "  Alive: " << player.second.alive;
      lines.push_back(os.str());
      os.clear();
      os.str("");

      os << "  Kills: " << player.second.kills;
      lines.push_back(os.str());
      os.clear();
      os.str("");
   }

   return lines;
}

void Scoreboard::OnRemoveScoreboard(const RemoveScoreboardEvent& event)
{
   (void) event;

   Invalidate();
}

void Scoreboard::OnCreatePlayer(const CreatePlayerEvent& event)
{
   mPlayers.insert({ event.GetPlayer(), PlayerInfo() });
}

void Scoreboard::OnKillPlayer(const KillPlayerEvent& event)
{
   for (auto& player : mPlayers)
   {
      if (event.GetVictim() == player.first)
      {
         player.second.alive = false;
         continue; // Suicide does not count as kill.
      }

      if (event.GetKiller() == player.first)
      {
         player.second.kills++;
      }
   }
}
