#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include "SceneObject.hpp"

#include <memory>
#include <string>
#include <vector>

class Arena;
class Player;

class Scoreboard : public SceneObject
{
public:
   Scoreboard();
   virtual ~Scoreboard();

   Scoreboard(const Scoreboard&) = delete;
   Scoreboard& operator=(const Scoreboard&) = delete;

   void Update(int elapsed_time) override;

   void KeepTrackOf(const std::shared_ptr<Arena>& arena);
   void KeepTrackOf(const std::vector<std::shared_ptr<Player>>& players);

   std::vector<std::string> GetScore() const;

private:
   std::shared_ptr<Arena> mArena = nullptr;
   std::vector<std::shared_ptr<Player>> mPlayers;
};

#endif // SCOREBOARD_HPP
