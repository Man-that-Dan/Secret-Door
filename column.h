#ifndef COLUMN__H
#define COLUMN__H
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "collisionStrategy.h"
#include "player.h"
#include <SDL.h>

class Column : public Drawable {
public:
//  Column(const int x);
  Column(Vector2f base[4], SDL_Color color);

  void draw() const;
  void update();
  void update(Uint32 ticks){
    int x = 0;
    x++;
  };
  //void collidedWith(Player& player);

  bool stopPlayer(float playerX, float playerY);
  int getScaledWidth() const;
  int getScaledHeight() const;
  const Image* getImage() const{
    Image* hello;
    return hello;
  };

  bool collidedWith( const Drawable* ){
     return false;
   };

  const SDL_Surface* getSurface() const{
    SDL_Surface* hello;
    return hello;
  };
  float clamp(float v, float low, float hi);
    SDL_Renderer * renderer;
  void drawSide(SDL_Renderer* renderer, SDL_Point p1, SDL_Point p2, SDL_Point p3, SDL_Point p4,
      SDL_Color color) const;

  void drawTop(SDL_Renderer* renderer, SDL_Point p1, SDL_Point p2, SDL_Point p3, SDL_Point p4,
      SDL_Color color) const;
  void setColor(SDL_Color newcol){
    colColor = newcol;
  };

protected:

  //counter clockwise points from top right
  Vector2f bases[4];
  Vector2f tops[4];
  Vector2f neutraltop[4];
  SDL_Color colColor;

  RectangularCollisionStrategy wall;


  const bool pequal(SDL_Point p1, SDL_Point p2) const{
    if(p1.x == p2.x && p2.y == p1.y)
      return true;
    return false;
  } const;

  int worldWidth;
  int worldHeight;


};
#endif
