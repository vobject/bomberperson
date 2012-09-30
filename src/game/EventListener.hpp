#ifndef EVENT_LISTENER_HPP
#define EVENT_LISTENER_HPP

class Event;

class EventListener
{
public:
   virtual void OnEvent(const Event& event) = 0;
};

#endif // EVENT_LISTENER_HPP
