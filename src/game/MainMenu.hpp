#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "SceneObject.hpp"
#include "../utils/Utils.hpp"

#include <string>
#include <vector>

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
//    void Choose();

//    bool HasChosen() const;
    UserInterfaceItem GetSelection() const;

private:
    unsigned int mSelectionMarker = 0;
//    bool mHasChosen = false;

    std::vector<UserInterfaceItem> mItems;
};

#endif // MAIN_MENU_HPP
