#ifndef TWOWAYMULTISPRITE__H
#define TWOWAYMULTISPRITE__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"

class CollisionStrategy;
class ExplodingSprite;

class TwowayMultiSprite : public Drawable {
public:
  TwowayMultiSprite(const std::string&, const std::string& direc, float xspeed, float yspeed);
  TwowayMultiSprite(const std::string&, const std::string& direc, float x, float y, float xspeed, float yspeed);
  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual bool collidedWith(const Drawable*) {
    throw std::string("I don't collide: ")+getName();
  }

  virtual void explode();

  virtual const Image* getImage() const {
    return images[currentFrame];
  }
  int getScaledWidth()  const {
    return getScale()*images[currentFrame]->getWidth();
  }
  int getScaledHeight()  const {
    return getScale()*images[currentFrame]->getHeight();
  }
  virtual const SDL_Surface* getSurface() const {
    return images[currentFrame]->getSurface();
  }
  void setMode(const std::string& newmode){
    mode = newmode;
  }
  void inform(const Vector2f playerinfo){
    player_loc = playerinfo;
  }
  void crashed(const int iswall){
    wall = iswall;
    if(iswall == 0){
      safepos = getPosition();
    };
  };
  TwowayMultiSprite(const TwowayMultiSprite&) = default;
  TwowayMultiSprite& operator=(const TwowayMultiSprite&) = default;

protected:
  std::vector<Image *> imagesRight;
  std::vector<Image *> imagesLeft;
  std::vector<Image *> images;
  ExplodingSprite* explosion;
  std::string mode;
  int boundary;
  std::string direction;
  Vector2f initial_state;
  Vector2f player_loc;
  int wall;
  Vector2f safepos;
  unsigned currentFrame;
  unsigned numberOfFrames;
  unsigned frameInterval;
  float timeSinceLastFrame;
  int worldWidth;
  int worldHeight;

  void advanceFrame(Uint32 ticks);
};
#endif
