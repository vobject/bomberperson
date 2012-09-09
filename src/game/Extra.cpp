#include "Extra.hpp"

Extra::Extra(const EntityId extra_id)
   : SceneObject(extra_id)
{
   SetZOrder(ZOrder::Layer_2);
}

Extra::~Extra()
{

}

void Extra::Update(const int elapsed_time)
{

}
