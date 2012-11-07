#ifndef PREPARE_GAME_MENU_HPP
#define PREPARE_GAME_MENU_HPP

#include "SceneObject.hpp"
#include "EventListener.hpp"

#include <vector>

class EventQueue;
class CreateMenuItemEvent;
class MenuInputEvent;
class MenuItemActionEvent;
enum class MenuItemId;

class PrepareGameMenu : public SceneObject, public EventListener
{
public:
    PrepareGameMenu(EventQueue& queue);
    virtual ~PrepareGameMenu();

    PrepareGameMenu(const PrepareGameMenu&) = delete;
    PrepareGameMenu& operator=(const PrepareGameMenu&) = delete;

    void Update(int elapsed_time) override;
    void OnEvent(const Event& event) override;

private:
    void OnCreateMenuItem(const CreateMenuItemEvent& event);
    void OnMenuInput(const MenuInputEvent& event);
    void OnMenuItemAction(const MenuItemActionEvent& event);

    void SelectionUp();
    void SelectionDown();
    void Choose(MenuItemId item);

    EventQueue& mEventQueue;

    bool mInputUp = false;
    bool mInputDown = false;
    bool mInputLeft = false;
    bool mInputRight = false;
    bool mInputEnter = false;
    bool mInputEscape = false;

    unsigned int mCurrentSelection = 0;
    std::vector<MenuItemId> mItems;
};

#endif // PREPARE_GAME_MENU_HPP
