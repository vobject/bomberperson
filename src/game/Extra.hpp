#ifndef EXTRA_HPP
#define EXTRA_HPP

#include "SceneObject.hpp"

class Extra : public SceneObject
{
public:
   Extra(EntityId extra_id);
   virtual ~Extra();

   Extra(const Extra&) = delete;
   Extra& operator=(const Extra&) = delete;

   void Update(int elapsed_time) override;
};

#endif // EXTRA_HPP
