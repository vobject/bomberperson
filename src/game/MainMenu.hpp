#ifndef MAIN_MENU_HPP
#define MAIN_MENU_HPP

#include "SceneObject.hpp"
#include "../utils/Utils.hpp"

#include <string>
#include <vector>

class EntityManager;
class MenuItem;
class MenuItemSelector;

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

class MainMenu : public SceneObject
{
public:
    MainMenu(EntityManager& entity_factory);
    virtual ~MainMenu();

    MainMenu(const MainMenu&) = delete;
    MainMenu& operator=(const MainMenu&) = delete;

    void Update(int elapsed_time) override;

    void SelectionUp();
    void SelectionDown();
    void Choose();

    std::shared_ptr<MenuItem> GetSelection();
    std::vector<std::shared_ptr<MenuItem>> GetMenuItems() const;
    MenuSound GetSound(bool reset);

    void SetResumeStatus(bool enabled);

private:
    EntityManager& mEntityFactory;

    unsigned int mSelectionMarker = 0;
    std::shared_ptr<MenuItemSelector> mSelector;
    std::vector<std::shared_ptr<MenuItem>> mItems;
    MenuSound mSound = MenuSound::None;
};

#endif // MAIN_MENU_HPP
