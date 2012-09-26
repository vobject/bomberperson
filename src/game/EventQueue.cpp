#include "EventQueue.hpp"

EventQueue::EventQueue()
{

}

EventQueue::~EventQueue()
{

}

void EventQueue::Register(EventListener* obj)
{
   mListeners.push_back(obj);
}

void EventQueue::UnRegister(EventListener* obj)
{
   mListeners.remove(obj);
}

void EventQueue::Add(const std::shared_ptr<Event>& event)
{
   mQueues[mCurrentQueue].push_back(event);
}

void EventQueue::ProcessEvents()
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

void EventQueue::SwapQueues()
{
   mCurrentQueue = (mCurrentQueue + 1) % QUEUE_COUNT;
}
