#ifndef USER_INTERFACE_HPP
#define USER_INTERFACE_HPP

#include "EventListener.hpp"
#include "EventQueue.hpp"
#include "EntityManager.hpp"
//#include "MainMenu.hpp"
#include "../input/KeyboardInput.hpp"
#include "../utils/Utils.hpp"

struct SDL_KeyboardEvent;
struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;

class BomberPersonConfig;
class MenuItemActionEvent;
//class MainMenu;
//class MenuItem;
//enum class MainMenuItem;

enum class MenuType
{
   MainMenu,
   PrepareGame,
//   Options
};

enum class MenuItemId
{
   MainMenu_ResumeGame,
   MainMenu_NewGame,
   MainMenu_PrepareGame,
////   MainMenu_Options,
////   MainMenu_Credits,
   MainMenu_Exit,

   PrepareGame_Arena,
   PrepareGame_ArenaSize,
   PrepareGame_PlayerCount,
   PrepareGame_Exit
};

enum class MenuMusic
{
   MainMenuTrack
};

enum class MenuSound
{
   None,
   Switch,
   Choose,
   Invalid
};

enum class ArenaId
{
   Arena_1,
   Arena_2,
   Arena_3
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
   ArenaId arena;
   std::vector<std::pair<PlayerId, InputId>> players;
   // Level
   // Key config
};

class UserInterface : public EventListener
{
public:
    UserInterface(BomberPersonConfig& cfg);
    ~UserInterface();

    UserInterface(const UserInterface&) = delete;
    UserInterface& operator=(const UserInterface&) = delete;

    void Input(const SDL_KeyboardEvent& key);
    void Input(const SDL_MouseMotionEvent& motion);
    void Input(const SDL_MouseButtonEvent& button);

    void Update(int elapsed_time);
    void OnEvent(const Event& event) override;

    bool IsActive() const;
    bool IsDone() const;
    MenuItemId GetSelection() const;

    void ShowMainMenu(bool game_paused);
    void HideMainMenu();

    MatchSettings GetMatchSettings() const;
    EntitySet GetEntities() const;

private:
   void OnMenuItemAction(const MenuItemActionEvent& event);

   void CreateInputEvents();
   void UpdateEntities(int elapsed_time);

   BomberPersonConfig& mConfig;
   EventQueue mEventQueue;
   EntityManager mEntityManager;
   KeyboardInput mKeyboard;

   bool mActive = false;
   bool mDone = false;
   MenuItemId mSelection = MenuItemId::MainMenu_NewGame;

   MatchSettings mSettings;
};

#endif // USER_INTERFACE_HPP
