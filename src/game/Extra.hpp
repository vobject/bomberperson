#ifndef EXTRA_HPP
#define EXTRA_HPP

#include "ArenaObject.hpp"

enum class ExtraType
{
   Speed,
   Bombs,
   Range,
   InfiniteRange,
   Kick,
   RemoteBombs,
   Slow
};

//enum class ExtraSound
//{

//};

class Extra : public ArenaObject
{
public:
   Extra(const std::shared_ptr<Arena>& arena, ExtraType type);
   virtual ~Extra();

   Extra(const Extra&) = delete;
   Extra& operator=(const Extra&) = delete;

   void Update(int elapsed_time) override;

   ExtraType GetType() const;

private:
   const ExtraType mType;
};

#endif // EXTRA_HPP
