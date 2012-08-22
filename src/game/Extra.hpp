#ifndef EXTRA_HPP
#define EXTRA_HPP

#include "SceneObject.hpp"

enum class ExtraType
{
   Speed,
   BombRange,
   BombSupply
};

class Extra : public SceneObject
{
public:
   Extra(const std::string& name, ExtraType type);
   virtual ~Extra();

   Extra(const Extra&) = delete;
   Extra& operator=(const Extra&) = delete;

   void Update(int elapsed_time) override;

   ExtraType GetType() const;

private:
   ExtraType mType;
};

#endif // EXTRA_HPP
