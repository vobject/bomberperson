#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "SceneObject.hpp"
#include "EventListener.hpp"
#include "../utils/Utils.hpp"

#include <memory>
#include <string>
#include <vector>

class EventQueue;
class CreateMainMenuItemEvent;
class MenuInputEvent;
class MenuItem;
class MenuItemSelector;

enum class MenuMusic
{
   MainmenuTrack
};

enum class MenuSound
{
   None,
   Switch,
   Choose,
   Invalid
};

enum class MainMenuItem
{
   ResumeGame,
   NewGame,
   Exit
};

class MainMenu : public SceneObject, public EventListener
{
public:
    MainMenu(EventQueue& queue);
    virtual ~MainMenu();

    MainMenu(const MainMenu&) = delete;
    MainMenu& operator=(const MainMenu&) = delete;

    void Update(int elapsed_time) override;
    void OnEvent(const Event& event) override;

//    std::shared_ptr<MenuItem> GetSelection();
//    std::vector<std::shared_ptr<MenuItem>> GetMenuItems() const;
    MenuSound GetSound(bool reset);

//    void SetResumeStatus(bool enabled);

private:
    void OnCreateMainMenuItem(const CreateMainMenuItemEvent& event);
    void OnMenuInput(const MenuInputEvent& event);

    void SelectionUp();
    void SelectionDown();
    void Choose(MainMenuItem item);
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
    std::vector<MainMenuItem> mItems;
    MenuSound mSound = MenuSound::None;
};

#endif // MAIN_MENU_HPP
