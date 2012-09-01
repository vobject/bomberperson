#include "Match.hpp"
#include "Arena.hpp"
#include "Cell.hpp"
#include "Player.hpp"
#include "../utils/Utils.hpp"

Match::Match(
   const std::shared_ptr<Arena>& arena,
   const std::vector<std::shared_ptr<Player>>& players
)
   : mArena(arena)
   , mPlayers(players)
{

}

Match::~Match()
{

}

void Match::Update(const int elapsed_time)
{
   // TODO: End game if all but one players are dead.

   mArena->Update(elapsed_time);

   auto player = std::begin(mPlayers);
   while (player != std::end(mPlayers))
   {
      if ((*player)->IsAlive())
      {
         (*player)->Update(elapsed_time);
         (*player)->SetParentCell(GetCellFromObject(*player));
         player++;
      }
      else
      {
         player = mPlayers.erase(player);
      }
   }
}

std::shared_ptr<Arena> Match::GetArena() const
{
   return mArena;
}

std::vector<std::shared_ptr<Player>> Match::GetPlayers() const
{
   return mPlayers;
}

std::shared_ptr<Cell> Match::GetCellFromObject(
   const std::shared_ptr<SceneObject>& obj
) const
{
   const Point pos = { obj->GetPosition().X + (obj->GetSize().Width / 2),
                       obj->GetPosition().Y + (obj->GetSize().Height / 2) };
   return mArena->GetCellFromPosition(pos);
}
