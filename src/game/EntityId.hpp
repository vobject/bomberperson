#ifndef ENTITY_ID_HPP
#define ENTITY_ID_HPP

// These turned out to be worthless.
enum class EntityId
{
   Menu,
   Arena,
   Scoreboard,
   Cell,
   Wall,
   Extra,
   Bomb,
   Explosion,
   Player
};

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
   Switch,
   Choose,
   Invalid
};

enum class ScoreboardType
{

};

// All types of game assets.
// Will be used as a bridge between game objects (SceneObject)
//  and resource management (ResCache).
// An object that can have its own resource (e.g. Sprite)
//  must get its own EntityId.
//enum class EntityId
//{
//   MainMenu,

//   Arena,

//   Scoreboard,

//   Cell,

//   IndestructibleWall,
//   DestructibleWall,

//   SpeedExtra,
//   BombsExtra,
//   RangeExtra,
//   GoldRangeExtra,

//   Bomb,

//   Explosion,

////enum class ExplosionType
////{
////   Crossway,
////   Vertical,
////   VerticalLeftEnd,
////   VerticalRightEnd,
////   Horizontal,
////   HorizontalLeftEnd,
////   HorizontalRightEnd
////};

//   Player_1,
//   Player_2,
//   Player_3,
//   Player_4
//};

#endif // ENTITY_ID_HPP
