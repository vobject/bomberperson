#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include "EntityManager.hpp"
#include "../utils/Utils.hpp"

#include <memory>
#include <vector>

struct SDL_KeyboardEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;

class MainMenu;
class MenuItem;

enum class UiItemId
{
   MainMenu_NewGame,
   MainMenu_ResumeGame,
//   MainMenu_SetupGame,
//   MainMenu_Options,
//   MainMenu_Credits,
   MainMenu_Exit
};

enum class InputId
{
   Keyboard_1,
   Keyboard_2,
   Mouse_1,
   Kinect_1
};

enum class PlayerId
{
   Player_1,
   Player_2,
   Player_3,
   Player_4
};

struct MatchSettings
{
   std::vector<std::pair<PlayerId, InputId>> players;
   // Level
   // Key config
};

class UserInterface
{
public:
    UserInterface();
    ~UserInterface();

    UserInterface(const UserInterface&) = delete;
    UserInterface& operator=(const UserInterface&) = delete;

    void Input(const SDL_KeyboardEvent& key);
    void Input(const SDL_MouseMotionEvent& motion);
    void Input(const SDL_MouseButtonEvent& button);

    void Update(int elapsed_time);

    bool IsActive() const;
    bool IsDone() const;

    void ShowMainMenu(bool game_paused);
    void HideMainMenu();

    UiItemId GetSelection() const;

    MatchSettings GetMatchSettings() const;
    EntitySet GetEntities() const;

private:
   EntityManager mEntityManager;
   MatchSettings mSettings;
   bool mActive = false;
   bool mDone = false;
   UiItemId mSelection;

   std::shared_ptr<MainMenu> mMainMenu;
};

#endif // USER_INTERFACE_HPP
