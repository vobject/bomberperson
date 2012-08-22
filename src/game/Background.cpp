#include "Background.hpp"

Background::Background(const std::string& name)
{
   SetResourceId(name);
}

Background::~Background()
{

}

void Background::Update(const int elapsed_time)
{

}

Size Background::GetBorderSize() const
{
   return mBorderSize;
}

void Background::SetBorderSize(const Size& size)
{
   mBorderSize = size;
}
