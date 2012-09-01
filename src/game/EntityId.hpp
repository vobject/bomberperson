#ifndef ENTITY_ID_HPP
#define ENTITY_ID_HPP

// All types of game assets.
// Will be used as a bridge between game objects (SceneObject)
//  and resource management (ResCache).
// An object that can have its own resource (e.g. Sprite)
//  must get its own EntityId.
enum class EntityId
{
   MainMenu,

   Arena,

   Cell,

   IndestructibleWall,
   DestructibleWall,

   SpeedExtra,
   BombsExtra,
   RangeExtra,

   Bomb,

   Explosion,

   Player_1,
   Player_2,
   Player_3,
   Player_4
};

#endif // ENTITY_ID_HPP
