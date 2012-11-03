#ifndef MENUITEM_HPP
#define MENUITEM_HPP

#include "SceneObject.hpp"
#include "EventListener.hpp"

#include <string>

class EventQueue;
class MainMenuSelectionEvent;
class MainMenuEnableEvent;
enum class MainMenuItem;

// FIXME: This class depends on MainMenuItem.
class MenuItem : public SceneObject, public EventListener
{
public:
   MenuItem(MainMenuItem type, EventQueue& queue);
   virtual ~MenuItem();

   MenuItem(const MenuItem&) = delete;
   MenuItem& operator=(const MenuItem&) = delete;

   void Update(int elapsed_time) override;
   void OnEvent(const Event& event) override;

   // TODO: This should not be part of the MenuItem class but should
   //  be loaded from the resources.
   std::string GetText() const;
   void SetText(const std::string& text);

   bool IsEnabled() const;
   void SetEnabled(bool status);

   bool IsSelected() const;
   void SetSelected(bool status);

private:
   void OnMainMenuSelection(const MainMenuSelectionEvent& event);
   void OnMainMenuEnabled(const MainMenuEnableEvent& event);

   const MainMenuItem mType;

   EventQueue& mEventQueue;

   bool mEnabled = false;
   bool mSelected = false;
   std::string mText;
};

#endif // MENUITEM_HPP
