#ifndef BOMB_HPP
#define BOMB_HPP

#include "ArenaObject.hpp"
#include "../utils/Utils.hpp"

class Player;
class EntityManager;
enum class ExplosionType;

enum class BombType
{
   Countdown,
   Remote
//   CountDownJelly,
//   RemoteJelly
};

enum class BombSound
{
   None,
   Planted
};

class Bomb : public ArenaObject
{
public:
   Bomb(const std::shared_ptr<Arena>& arena,
        BombType type,
        const std::shared_ptr<Player>& owner,
        EntityManager& entity_factory);
   virtual ~Bomb();

   Bomb(const Bomb&) = delete;
   Bomb& operator=(const Bomb&) = delete;

   void Update(int elapsed_time) override;

   BombType GetType() const;
   BombSound GetSound(bool reset);

   int GetRange() const;
   void SetRange(int range);

   bool CanMove(Direction dir, int distance) const;
   void Move(Direction dir, int speed, int distance);
   void Detonate();

private:
   void PlantCenterExplosion() const;
   void PlantRangeExplosion(Direction dir) const;
   ExplosionType GetExplosionType(Direction dir, bool end) const;

   const BombType mType;
   const std::shared_ptr<Player> mOwner;
   EntityManager& mEntityFactory;

   int mLifeTime = 0_ms;
   int mRange = 1;
   BombSound mSound = BombSound::None;

   bool mIsMoving = false;
   int mMoveIdleTime = 0_ms;
   Direction mMoveDirection;
   int mMoveSpeed;
   int mMoveDistance;
};

#endif // BOMB_HPP
