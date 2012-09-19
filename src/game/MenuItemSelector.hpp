#ifndef MENUITEMSELECTOR_HPP
#define MENUITEMSELECTOR_HPP

#include "SceneObject.hpp"

class MenuItemSelector : public SceneObject
{
public:
   MenuItemSelector();
   virtual ~MenuItemSelector();

   MenuItemSelector(const MenuItemSelector&) = delete;
   MenuItemSelector& operator=(const MenuItemSelector&) = delete;

   void Update(int elapsed_time) override;
};

#endif // MENUITEMSELECTOR_HPP
