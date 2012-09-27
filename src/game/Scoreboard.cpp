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
      case EventType::SpawnPlayerStart:
         OnSpawnPlayerStart(dynamic_cast<const SpawnPlayerStartEvent&>(event));
         break;
      case EventType::DestroyPlayerStart:
         OnDestroyPlayerStart(dynamic_cast<const DestroyPlayerStartEvent&>(event));
         break;
//      case EventType::CreateBomb:
//         OnCreateBomb(dynamic_cast<const CreateBombEvent&>(event));
//         break;
//      case EventType::CreateExplosion:
//         OnCreateExplosion(dynamic_cast<const CreateExplosionEvent&>(event));
//         break;
//      case EventType::Input:
//         OnInput(dynamic_cast<const InputEvent&>(event));
//         break;
//      case EventType::MovePlayer:
//         OnMovePlayer(dynamic_cast<const MovePlayerEvent&>(event));
//         break;
//      case EventType::PickupExtra:
//         OnPickupExtra(dynamic_cast<const PickupExtraEvent&>(event));
//         break;
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

void Scoreboard::OnSpawnPlayerStart(const SpawnPlayerStartEvent& event)
{
   mPlayers.insert({ event.GetPlayer(), PlayerInfo() });
}

void Scoreboard::OnDestroyPlayerStart(const DestroyPlayerStartEvent& event)
{
   for (auto& player : mPlayers)
   {
      if (event.GetPlayer() == player.first)
      {
         player.second.alive = false;
         continue;
      }

      if (event.GetKiller() == player.first)
      {
         player.second.kills++;
      }
   }
}
