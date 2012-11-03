#ifndef MENUITEM_HPP
#define MENUITEM_HPP

#include "SceneObject.hpp"
#include "EventListener.hpp"

#include <string>

class EventQueue;
class MenuItemSelectionEvent;
class MenuItemEnableEvent;
enum class MenuItemId;

class MenuItem : public SceneObject, public EventListener
{
public:
   MenuItem(MenuItemId type, EventQueue& queue);
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
   void OnMenuItemSelection(const MenuItemSelectionEvent& event);
   void OnMenuItemEnabled(const MenuItemEnableEvent& event);

   const MenuItemId mType;

   EventQueue& mEventQueue;

   bool mEnabled = false;
   bool mSelected = false;
   std::string mText;
};

#endif // MENUITEM_HPP
