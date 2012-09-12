#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "SceneObject.hpp"
#include "../utils/Utils.hpp"

#include <string>
#include <vector>

enum class MenuType
{
   Mainmenu
};

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

//enum class MainMenuItem
//{
//   StartGame,
////   SetupGame,
////   Options,
////   Credits,
//   Exit
//};

class UserInterfaceItem;

class MainMenu : public SceneObject
{
public:
    MainMenu();
    virtual ~MainMenu();

    MainMenu(const MainMenu&) = delete;
    MainMenu& operator=(const MainMenu&) = delete;

    void Update(int elapsed_time) override;

    void AddMenuItem(const UserInterfaceItem& item);
    std::vector<UserInterfaceItem> GetMenuItems() const;

    void SelectionUp();
    void SelectionDown();
    void Choose();

    UserInterfaceItem GetSelection();
    MenuSound GetSound(bool reset);

private:
    unsigned int mSelectionMarker = 0;
    MenuSound mSound = MenuSound::None;
    std::vector<UserInterfaceItem> mItems;
};

#endif // MAIN_MENU_HPP
