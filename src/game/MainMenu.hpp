#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "SceneObject.hpp"
#include "EventListener.hpp"

#include <vector>

class EventQueue;
class CreateMenuItemEvent;
class MenuInputEvent;
enum class MenuItemId;
enum class MenuSound;

class MainMenu : public SceneObject, public EventListener
{
public:
    MainMenu(EventQueue& queue);
    virtual ~MainMenu();

    MainMenu(const MainMenu&) = delete;
    MainMenu& operator=(const MainMenu&) = delete;

    void Update(int elapsed_time) override;
    void OnEvent(const Event& event) override;

    MenuSound GetSound(bool reset);

private:
    void OnCreateMenuItem(const CreateMenuItemEvent& event);
    void OnMenuInput(const MenuInputEvent& event);

    void SelectionUp();
    void SelectionDown();
    void Choose(MenuItemId item);
    void ResetInputState();

    EventQueue& mEventQueue;

    // Input handling:
    bool mInputUp = false;
    bool mInputDown = false;
    bool mInputLeft = false;
    bool mInputRight = false;
    bool mInputEnter = false;
    bool mInputEscape = false;

    unsigned int mCurrentSelection = 0;
    std::vector<MenuItemId> mItems;
    MenuSound mSound;
};

#endif // MAIN_MENU_HPP
