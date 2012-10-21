#ifndef EXTRA_RESOURCE_HPP
#define EXTRA_RESOURCE_HPP

#include <vector>

enum class ExtraType;

struct SDL_Surface;

// TODO: Add support for extra animations like 'random'.

class ExtraResource
{
public:
   ExtraResource(ExtraType type, const std::vector<SDL_Surface*>& textures);
   ~ExtraResource();

   ExtraResource(const ExtraResource&) = default;
   ExtraResource& operator=(const ExtraResource&) = delete;

   ExtraType GetType() const;

   SDL_Surface* GetFrame() const;

private:
   const ExtraType mType;
   const std::vector<SDL_Surface*> mFrames;
};

#endif // EXTRA_RESOURCE_HPP
