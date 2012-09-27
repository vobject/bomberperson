#ifndef EVENT_QUEUE_HPP
#define EVENT_QUEUE_HPP

#include <list>
#include <memory>
#include <vector>

class Event;
class EventListener;

class EventQueue
{
public:
   EventQueue();
   ~EventQueue();

   void Register(EventListener* obj);
   void UnRegister(EventListener* obj);

   void Add(const std::shared_ptr<Event>& event);

   void ProcessEvents();

private:
   void SwapQueues();

   // Objects (basically all) that want to know about enqueued events.
   std::list<EventListener*> mListeners;

   static const int QUEUE_COUNT = 2;
   std::vector<std::shared_ptr<Event>> mQueues[QUEUE_COUNT];
   int mCurrentQueue = 0;
};

#endif // EVENT_QUEUE_HPP
