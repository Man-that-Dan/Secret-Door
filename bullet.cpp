#include <iostream>
#include <cmath>
#include "bullet.h"
#include "gamedata.h"
#include "imageFactory.h"

Bullet::Bullet( const string& name, const Vector2f&  pos,
                          const Vector2f& vel) :
   MultiSprite(name, pos, vel),
   distance(0),
   maxDistance(Gamedata::getInstance().getXmlInt(name+"/distance")),
   tooFar(false),
   swing(0)
{ }

void Bullet::reset() {
  tooFar = false;
  distance = 0;
}

void Bullet::update(Uint32 ticks) {
  Vector2f pos = getPosition();
  MultiSprite::update(ticks);
  if ( getY()+getScaledHeight() < 0 || getY() > worldHeight ) {
    tooFar = true;
  }
  if ( getX() < 0 || getX()+getScaledWidth() > worldWidth ) {
    tooFar = true;
  }
  distance += ( hypot(getX()-pos[0], getY()-pos[1]) );
  if (distance > maxDistance) tooFar = true;
  swing++;
  MultiSprite::advanceFrame(ticks/200000);
  if(swing > 10){
    swing = 0;
    tooFar = true;
  };
}
