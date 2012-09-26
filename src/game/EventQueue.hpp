#ifndef EVENT_QUEUE_HPP
#define EVENT_QUEUE_HPP

#include "SceneObject.hpp"
#include "../utils/Utils.hpp"

#include <algorithm>
#include <vector>

enum class PlayerType;

enum class EventType
{
   MovePlayer,
//   PickupExtra,
//   PlayerDeath,

};

class Event
{
public:
   Event(EventType type) : mType(type) { }
   virtual ~Event() { }

   EventType GetType() const { return mType; }

private:
   const EventType mType;
};

class MovePlayerEvent : public Event
{
public:
   MovePlayerEvent(PlayerType player, const std::vector<std::pair<Direction, int>>& dir)
      : Event(EventType::MovePlayer)
      , mPlayer(player)
      , mDirectionDistancePair(dir)
   { }
   virtual ~MovePlayerEvent() { }

   PlayerType GetPlayerType() const { return mPlayer; }
   std::vector<std::pair<Direction, int>> GetMovementData() const { return mDirectionDistancePair; }

private:
   const PlayerType mPlayer;
   const std::vector<std::pair<Direction, int>> mDirectionDistancePair;
};

class EventQueue
{
public:
   EventQueue();
   ~EventQueue();

   void Register(SceneObject* obj)
   {
      const auto iter = std::find(mListeners.begin(), mListeners.end(), obj);
      if (mListeners.end() != iter) {
         throw "Trying to register an existing listener to the event queue";
      }
      mListeners.push_back(obj);
   }

   void UnRegister(SceneObject* obj)
   {
      const auto iter = std::find(mListeners.begin(), mListeners.end(), obj);
      if (mListeners.end() == iter) {
         throw "Trying to unregister unknown listener from event queue";
      }
      mListeners.erase(iter);
   }

   void Add(const std::shared_ptr<Event>& event)
   {
      mQueues[mCurrentQueue].push_back(event);
   }

   void ProcessEvents()
   {
      auto& current_queue = mQueues[mCurrentQueue];
      SwapQueues();

      for (auto& event : current_queue)
      {
         for (auto& listener : mListeners)
         {
            listener->OnEvent(*event);
         }
      }
      current_queue.clear();
   }

private:
   void SwapQueues()
   {
      mCurrentQueue = (mCurrentQueue + 1) % QUEUE_COUNT;
   }

   // Objects (basically all) that want to know about enqueued events.
   std::vector<SceneObject*> mListeners;

   static const int QUEUE_COUNT = 2;
   std::vector<std::shared_ptr<Event>> mQueues[QUEUE_COUNT];
   int mCurrentQueue = 0;
};

#endif // EVENT_QUEUE_HPP
