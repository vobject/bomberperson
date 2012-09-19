#ifndef MENUITEM_HPP
#define MENUITEM_HPP

#include "SceneObject.hpp"

#include <string>

enum class UiItemId;

class MenuItem : public SceneObject
{
public:
   MenuItem(UiItemId id);
   virtual ~MenuItem();

   MenuItem(const MenuItem&) = delete;
   MenuItem& operator=(const MenuItem&) = delete;

   void Update(int elapsed_time) override;

   UiItemId GetItemId() const;

   bool IsEnabled() const;
   void SetEnabled(bool status);

   // TODO: This should not be part of the MenuItem class but should
   //  be loaded from the resources.
   std::string GetText() const;
   void SetText(const std::string& text);

private:
   const UiItemId mId;
   bool mEnabled = true;
   bool mSelected = false;
   std::string mText;
};

#endif // MENUITEM_HPP
