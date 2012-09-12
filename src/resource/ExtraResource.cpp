#include "ExtraResource.hpp"

ExtraResource::ExtraResource(
   const ExtraType type,
   const std::vector<SDL_Surface*>& textures
)
   : mType(type)
   , mFrames(textures)
{

}

ExtraResource::~ExtraResource()
{

}


ExtraType ExtraResource::GetType() const
{
   return mType;
}

SDL_Surface *ExtraResource::GetFrame() const
{
   return mFrames.at(0);
}

