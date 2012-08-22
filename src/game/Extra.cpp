#include "Extra.hpp"

Extra::Extra(const std::string& name, const ExtraType type)
   : mType(type)
{
   SetResourceId(name);
}

Extra::~Extra()
{

}

void Extra::Update(const int elapsed_time)
{

}

ExtraType Extra::GetType() const
{
   return mType;
}
