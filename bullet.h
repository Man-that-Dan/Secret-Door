#ifndef BULLET__H
#define BULLET__H


#include "multisprite.h"
class Bullet : public MultiSprite {
public:
  Bullet( const std::string&, const Vector2f&, const Vector2f& );
  virtual void update(Uint32 ticks);
  bool goneTooFar() const { return tooFar; }
  void reset();

private:
  float distance;
  float maxDistance;
  bool tooFar;
  int swing;
};
#endif
