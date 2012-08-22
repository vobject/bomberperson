#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include "SceneObject.hpp"
#include "../utils/Utils.hpp"

class Background : public SceneObject
{
public:
   Background(const std::string& name);
   virtual ~Background();

   Background(const Background&) = delete;
   Background& operator=(const Background&) = delete;

   void Update(int elapsed_time) override;
};

#endif // BACKGROUND_HPP
