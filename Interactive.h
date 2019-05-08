#ifndef INTERACTIVE__H
#define INTERACTIVE__H
#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include "vector2f.h"
#include "image.h"
#include "drawable.h"



class Interactive : public Drawable {
public:

  Interactive(const std::string& n, const Vector2f& pos, bool hasImage, bool isPuzzle, std::string messg, int solution);



  ~Interactive() {}

  virtual void draw() const;
  virtual void update(Uint32 ticks);
  virtual bool shot(const Drawable*) {
    std::cout << "I can't be shot: " << std::endl;
    return false;
  };
  virtual void shoot() {
    std::cout << "I don't shoot: " << std::endl;
  };
  virtual bool collidedWith( const Drawable* ) {
    std::cout << "I don't collide: " << std::endl;
    return false;
  };
  virtual void explode() {
    std::cout << "I don't explode: " << std::endl;
  };



  virtual const Image* getImage() const { return image; }
  virtual const SDL_Surface* getSurface() const {
    return image->getSurface();
  }
  int getScaledWidth()  const { return Drawable::getScale()*image->getWidth();  }
  int getScaledHeight() const { return Drawable::getScale()*image->getHeight(); }
  bool isSolved() const{
    return solved;
  };
  bool takeScreen() const{
    return active;
  };
  void toggle();
  void turnLeft();
  void turnRight();






private:
  const Image * image;
  std::vector<Image *> images;

  int worldWidth;
  int worldHeight;
  int viewHeight;
  int viewWidth;
  bool imageIncluded;
  bool puzzle;
  bool solved;
  bool active;
  std::string message;
  int currentFrame;
  int solvestate;
};
#endif
