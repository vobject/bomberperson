#ifndef MATCH_HPP
#define MATCH_HPP

#include <memory>
#include <tuple>
#include <vector>

class Arena;
class Player;
class Scoreboard;
class Cell;
class SceneObject;

class Match
{
public:
   Match(const std::shared_ptr<Arena>& arena,
         const std::vector<std::shared_ptr<Player>>& players);
   virtual ~Match();

   Match(const Match&) = delete;
   Match& operator=(const Match&) = delete;

   void Update(int elapsed_time);

   std::shared_ptr<Arena> GetArena() const;
   std::vector<std::shared_ptr<Player>> GetPlayers() const;
   std::shared_ptr<Scoreboard> GetScoreboard() const;

   bool IsOver() const;

private:
   std::shared_ptr<Cell> GetCellFromObject(const std::shared_ptr<SceneObject>& obj) const;

   std::shared_ptr<Arena> mArena;
   std::vector<std::shared_ptr<Player>> mPlayers;
   std::shared_ptr<Scoreboard> mScoreboard;
};

#endif // MATCH_HPP
