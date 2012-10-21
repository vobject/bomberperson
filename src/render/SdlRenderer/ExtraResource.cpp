#include "ExtraResource.hpp"

ExtraResource::ExtraResource(
   const ExtraType type,
   const std::vector<SDL_Surface*>& textures
)
   : mType(type)
   , mFrames(textures)
{
   if (mFrames.empty()) {
      // An animation must always consist of at least one frame.
      throw "ExtraResource: Invalid sprite textures.";
   }
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
