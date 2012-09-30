#ifndef WALL_HPP
#define WALL_HPP

#include "ArenaObject.hpp"
#include "EventListener.hpp"

class EventQueue;
class RemoveWallEvent;

enum class WallType
{
   Indestructible,
   Destructible
};

//enum class WallSound
//{

//};

class Wall : public ArenaObject, public EventListener
{
public:
   Wall(const std::shared_ptr<Arena>& arena,
        WallType type,
        EventQueue& queue);
   virtual ~Wall();

   Wall(const Wall&) = delete;
   Wall& operator=(const Wall&) = delete;

   void Update(int elapsed_time) override;
   void OnEvent(const Event& event) override;

   WallType GetType() const;

private:
   void OnRemoveWall(const RemoveWallEvent& event);

   const WallType mType;

   EventQueue& mEventQueue;
};

#endif // WALL_HPP
