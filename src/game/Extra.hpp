#ifndef EXTRA_HPP
#define EXTRA_HPP

#include "ArenaObject.hpp"
#include "EventListener.hpp"

class EventQueue;
class RemoveExtraEvent;

enum class ExtraType
{
   Speed,
   Bombs,
   Range,
   InfiniteRange,
   Kick,
   RemoteBombs,
   Slow
};

//enum class ExtraSound
//{

//};

class Extra : public ArenaObject, public EventListener
{
public:
   Extra(const std::shared_ptr<Arena>& arena,
         ExtraType type,
         EventQueue& queue);
   virtual ~Extra();

   Extra(const Extra&) = delete;
   Extra& operator=(const Extra&) = delete;

   void Update(int elapsed_time) override;
   void OnEvent(const Event& event) override;

   ExtraType GetType() const;

private:
   void OnRemoveExtra(const RemoveExtraEvent& event);

   const ExtraType mType;

   EventQueue& mEventQueue;
};

#endif // EXTRA_HPP
