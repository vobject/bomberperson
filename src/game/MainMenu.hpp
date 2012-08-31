#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "SceneObject.hpp"
#include "../utils/Utils.hpp"

#include <string>
#include <vector>

enum class MainMenuItem
{
   StartGame,
//   SetupGame,
//   Options,
//   Credits,
   Exit
};

class MainMenu : public SceneObject
{
public:
    MainMenu(const std::string& name);
    virtual ~MainMenu();

    MainMenu(const MainMenu&) = delete;
    MainMenu& operator=(const MainMenu&) = delete;

    void Update(int elapsed_time) override;

    bool HasChosen() const;
    MainMenuItem GetSelection() const;

    void SelectionUp();
    void SelectionDown();
    void Choose();

private:
    const MainMenuItem FIRST_ITEM = MainMenuItem::StartGame;
    const MainMenuItem LAST_ITEM = MainMenuItem::Exit;

    MainMenuItem mSelection = FIRST_ITEM;
    bool mHasChosen = false;
};

#endif // MAINMENU_HPP
