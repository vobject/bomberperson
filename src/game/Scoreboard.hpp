#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include "SceneObject.hpp"
#include "EventListener.hpp"
#include "../utils/Utils.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>

class EventQueue;
class RemoveScoreboardEvent;
class CreatePlayerEvent;
class KillPlayerEvent;
enum class PlayerType;

//enum class ScoreboardType
//{

//};

class Scoreboard : public SceneObject, public EventListener
{
public:
   Scoreboard(EventQueue& queue);
   virtual ~Scoreboard();

   Scoreboard(const Scoreboard&) = delete;
   Scoreboard& operator=(const Scoreboard&) = delete;

   void Update(int elapsed_time) override;
   void OnEvent(const Event& event) override;

   std::vector<std::string> GetScore() const;

private:
   void OnRemoveScoreboard(const RemoveScoreboardEvent& event);
   void OnCreatePlayer(const CreatePlayerEvent& event);
   void OnKillPlayer(const KillPlayerEvent& event);

   struct PlayerInfo
   {
      bool alive = true;

//      int speed = 0;
//      int distance = 0;
//      int supply = 0;
//      int range = 0;
//      int remotes = 0;
//      bool kick = false;

//      int planted = 0;
      int kills = 0;
   };

   EventQueue& mEventQueue;

   int mGameTime = 0_ms;
   std::map<PlayerType, PlayerInfo> mPlayers;
};

#endif // SCOREBOARD_HPP
