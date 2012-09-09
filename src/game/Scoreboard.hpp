#ifndef SCOREBOARD_HPP
#define SCOREBOARD_HPP

#include "SceneObject.hpp"
#include "../utils/Utils.hpp"

#include <memory>
#include <string>
#include <vector>

class Player;

class Scoreboard : public SceneObject
{
public:
   Scoreboard();
   virtual ~Scoreboard();

   Scoreboard(const Scoreboard&) = delete;
   Scoreboard& operator=(const Scoreboard&) = delete;

   void Update(int elapsed_time) override;

   void KeepTrackOf(const std::shared_ptr<Player>& player);

   std::vector<std::string> GetScore() const;

private:
   int mGameTime = 0_ms;
   std::vector<std::shared_ptr<Player>> mPlayers;
};

#endif // SCOREBOARD_HPP
