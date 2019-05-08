#include <iostream>
#include <cmath>
#include "bullets.h"
#include "imageFactory.h"
#include "collisionStrategy.h"
//was using this for bullets, will now be sword, to save time I won't be renaming the class
// I'll do that later when I have time

Bullets::Bullets(const std::string& n) :
  name(n),
  myVelocity(
    Gamedata::getInstance().getXmlInt(name+"/speed/x"),
    Gamedata::getInstance().getXmlInt(name+"/speed/y")
  ),
  bulletList(),
  FreeList(),
  free(1),
  strategy(NULL)
{
  const string thisStrategy =
    Gamedata::getInstance().getXmlStr("collisionStrategy");
  if ( thisStrategy == "PerPixel" ) {
    strategy = new PerPixelCollisionStrategy;
  }
  else if ( thisStrategy == "Rectangular" ) {
    strategy = new RectangularCollisionStrategy;
  }
  else if ( thisStrategy == "MidPoint" ) {
    strategy = new MidPointCollisionStrategy;
  }
  //give 5 bulletw
  for(int i = 0; i < 5; i++){
    FreeList.push_back(Bullet(name, Vector2f(0, 0), Vector2f(0, 0)));
  };
}

void Bullets::shoot(const Vector2f& pos, const Vector2f& objVel) {
  if(free > 0){
    FreeList.at(5 - free).setPosition(pos);
    FreeList.at(5 - free).setVelocity(Vector2f(0, 0));
    //Bullet b( name,  pos, objVel );
    bulletList.push_back(FreeList.at(5 - free));
    free--;
  };
}

bool Bullets::collidedWith(const Drawable* obj) const {
  for ( const auto & bullet : bulletList ) {
    if ( strategy->execute(bullet, *obj) ) {
      return true;
    }
  }
  return false;
}

void Bullets::draw() const {
  for ( const auto& bullet : bulletList ) {
    bullet.draw();
  }
}

void Bullets::update(int ticks) {
  std::list<Bullet>::iterator ptr = bulletList.begin();
  while (ptr != bulletList.end()) {
    ptr->update(ticks);
    if (ptr->goneTooFar()) {  // Check to see if we should free a chunk
      ptr = bulletList.erase(ptr);
      free++;
    }
    else ++ptr;
  }
}
