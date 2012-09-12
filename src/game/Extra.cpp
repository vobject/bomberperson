#include "Extra.hpp"

Extra::Extra(const std::shared_ptr<Arena>& arena, const ExtraType type)
   : ArenaObject(EntityId::Extra, ZOrder::Layer_2, arena)
   , mType(type)
{

}

Extra::~Extra()
{

}

void Extra::Update(const int elapsed_time)
{
   (void) elapsed_time;
}


ExtraType Extra::GetType() const
{
   return mType;
}
