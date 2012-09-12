#ifndef ARENAOBJECT_HPP
#define ARENAOBJECT_HPP

#include "SceneObject.hpp"

#include <memory>

class Arena;

// Base class for objects that belong to an arena and therefore get
//  a reference to the arena they belong to.
class ArenaObject : public SceneObject
{
public:
   ArenaObject(EntityId id,
               ZOrder zorder,
               const std::shared_ptr<Arena>& arena);
   virtual ~ArenaObject();

   // TODO:
   // Die(); Request death/destruction animation.

protected:
   std::shared_ptr<Arena> GetArena() const;

private:
   const std::shared_ptr<Arena> mArena;
};

#endif // ARENAOBJECT_HPP
