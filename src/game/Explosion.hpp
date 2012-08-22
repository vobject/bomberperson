#ifndef EXPLOSION_HPP
#define EXPLOSION_HPP

#include "SceneObject.hpp"
#include "Animation.hpp"
#include "../utils/Utils.hpp"

enum class ExplosionType
{
   Crossway,
   Vertical,
   VerticalLeftEnd,
   VerticalRightEnd,
   Horizontal,
   HorizontalLeftEnd,
   HorizontalRightEnd
};

class Explosion : public SceneObject
{
public:
   Explosion(const std::string& name, ExplosionType type);
   virtual ~Explosion();

   Explosion(const Explosion&) = delete;
   Explosion& operator=(const Explosion&) = delete;

   void Update(int elapsed_time) override;

   ExplosionType GetType() const;
   int GetAnimationFrame() const;

private:
   static const int DEFAULT_LIFETIME = 1000_ms;

   ExplosionType mType;
   int mLifeTime = 0;

   Animation mAnimation;
};

#endif // EXPLOSION_HPP
