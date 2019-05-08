#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "drawable.h"
#include "column.h"
#include "player.h"
#include "renderContext.h"
#include "gamedata.h"
#include "collisionStrategy.h"
#include "viewport.h"



Column::Column(Vector2f base[4], SDL_Color color) :
Drawable("wall", base[0],
         Vector2f(0, 0)
         )
{
  renderer = RenderContext::getInstance()->getRenderer();
  for(int i = 0; i < 4; i++){
    bases[i] = base[i];
  };
  //top right
  tops[0][0] = bases[0][0] + 50;
  tops[0][1] = bases[0][1] - 50;
  //top left
  tops[1][0] = bases[1][0] - 50;
  tops[1][1] = bases[1][1] - 50;

  tops[2][0] = bases[2][0] - 50;
  tops[2][1] = bases[2][1] + 50;

  tops[3][0] = bases[3][0] + 50;
  tops[3][1] = bases[3][1] + 50;

  // //top right
  // tops[0][0] = bases[0][0] - 50;
  // tops[0][1] = bases[0][1] - 100;
  // //top left
  // tops[1][0] = bases[1][0] - 100;
  // tops[1][1] = bases[1][1] - 100;
  //
  // tops[2][0] = bases[2][0] - 150;
  // tops[2][1] = bases[2][1] - 50;
  //
  // tops[3][0] = bases[3][0] - 50;
  // tops[3][1] = bases[3][1] - 50;

  for(int i = 0; i < 4; i++){
    neutraltop[i] = tops[i];
  };

  colColor = color;
  worldWidth = (Gamedata::getInstance().getXmlInt("world/width"));
  worldHeight = (Gamedata::getInstance().getXmlInt("world/height"));

}

float Column::clamp(float v, float low, float hi){
  if(v < low)
    return low;
  if(v > hi)
    return hi;
  return v;
}

void Column::update(){
    // if(wall.execute(player, *this)){
    //   player.position = resetPos;
    //   player.stop();
    // };
    int x = Viewport::getInstance().getX();
    int y = Viewport::getInstance().getY();
    for(int i = 0; i < 4; i++){
      //perspective update
    //  std::cout << player.position[0] << " " << player.position[1] << std::endl;
      tops[i][1] = clamp((neutraltop[i][1] - ((y + 200) - bases[0][1])), neutraltop[i][1] - 300, neutraltop[i][1] + 300);
      tops[i][0] = clamp((neutraltop[i][0] - ((x + 400) - bases[0][0])), neutraltop[i][0] - 300, neutraltop[i][0] + 300);
      if(tops[i][0] == bases[i][0])
        tops[i][0]--;
    };

  }

void Column::draw() const {
  int basepoints[4][2];
  int toppoints[4][2];
  for(int i = 0; i < 4; i++){
    basepoints[i][0] = bases[i][0] - Viewport::getInstance().getX();
    basepoints[i][1] = bases[i][1] - Viewport::getInstance().getY();
    toppoints[i][0] = tops[i][0] - Viewport::getInstance().getX();
    toppoints[i][1] = tops[i][1] - Viewport::getInstance().getY();

  };
  std::cout << toppoints[0][0] << std::endl;

  SDL_Point points[2][4];
  for(int i = 0; i < 4; i++){
    points[0][i].x = basepoints[i][0];
    points[0][i].y = basepoints[i][1];
  };
  for(int i = 0; i < 4; i++){
    points[1][i].x = toppoints[i][0];
    points[1][i].y = toppoints[i][1];
  };
  //draw the column
  SDL_Color color2 = {118, 57, 40, colColor.a};
  SDL_Color topcolor = {0, 0, 0, 255};
  //draw right side vs left
  if(toppoints[2][0] < basepoints[2][0]){
    //draw top side vs bottom
    if(toppoints[2][1] < basepoints[2][1]){


      //right side
      drawSide(renderer, points[0][0], points[1][0], points[1][3], points[0][3], color2);

      //bottom side
      drawTop(renderer, points[1][3], points[1][2], points[0][2], points[0][3], colColor);

      //always here, this is the top
      drawTop(renderer, points[1][0], points[1][1], points[1][2], points[1][3], topcolor);
    } else {

      //top side
      drawTop(renderer, points[0][0], points[0][1], points[1][1], points[1][0], colColor);


      //right side
      drawSide(renderer, points[0][0], points[1][0], points[1][3], points[0][3], color2);


      //always here, this is the top view down
      drawTop(renderer, points[1][0], points[1][1], points[1][2], points[1][3], topcolor);
    };

  } else {
    //draw top side vs bottom
    if(toppoints[2][1] < basepoints[2][1]){
      //left side
      drawSide(renderer, points[1][1], points[0][1], points[0][2], points[1][2], color2);

      //bottom side
      drawTop(renderer, points[1][3], points[1][2], points[0][2], points[0][3], colColor);

      //always here, this is the top
      drawTop(renderer, points[1][0], points[1][1], points[1][2], points[1][3], topcolor);
    } else {

      //top side
      drawTop(renderer, points[0][0], points[0][1], points[1][1], points[1][0], colColor);


      //left side
      drawSide(renderer, points[1][1], points[0][1], points[0][2], points[1][2], color2);



      //always here, this is the top view down
      drawTop(renderer, points[1][0], points[1][1], points[1][2], points[1][3], topcolor);
    };
  };
  // x -= Viewport::getInstance().getX();
  // y -= Viewport::getInstance().getY();

  // SDL_Rect dest =  {x, y, tempWidth, tempHeight};
  // SDL_RenderCopy(renderer, texture, &view, &dest);
}

void Column::drawTop(SDL_Renderer* renderer, SDL_Point p1, SDL_Point p2, SDL_Point p3, SDL_Point p4,
   SDL_Color color) const{
     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
     SDL_Point t1, t2;
     t1 = p2;
     t2 = p3;
     float slope1 = (p2.x > p1.x) ? ((p2.y - p1.y) / (p2.x - p1.x)) : ((p1.y - p2.y) / (p1.x - p2.x));
     float slope2 = (p4.x > p3.x) ? ((p4.y - p3.y) / (p4.x - p3.x)) : ((p3.y - p4.y) / (p3.x - p4.x));
  
     SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
     SDL_RenderDrawLine(renderer, p2.x, p2.y, p3.x, p3.y);
     SDL_RenderDrawLine(renderer, p3.x, p3.y, p4.x, p4.y);
     SDL_RenderDrawLine(renderer, p4.x, p4.y, p1.x, p1.y);


     while(!(pequal(t1, p1) && pequal(t2, p4))){
       if(!pequal(t1, p1)){
         t1.x++;
       };
       if(!pequal(t2, p4)){
         t2.x++;
       };

       SDL_RenderDrawLine(renderer, t1.x, t1.y, t2.x, t2.y);
     };


   };

   //used for drawing sides of column, p1 is top right point
void Column::drawSide(SDL_Renderer* renderer, SDL_Point p1, SDL_Point p2, SDL_Point p3, SDL_Point p4,
  SDL_Color color) const{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Point t1, t2;
    t1 = p1;
    t2 = p2;
    float slope1 = (p2.x > p1.x) ? ((p2.y - p1.y) / (p2.x - p1.x)) : ((p1.y - p2.y) / (p1.x - p2.x));
    float slope2 = (p4.x > p3.x) ? ((p4.y - p3.y) / (p4.x - p3.x)) : ((p3.y - p4.y) / (p3.x - p4.x));

    SDL_RenderDrawLine(renderer, p1.x, p1.y, p2.x, p2.y);
    SDL_RenderDrawLine(renderer, p2.x, p2.y, p3.x, p3.y);
    SDL_RenderDrawLine(renderer, p3.x, p3.y, p4.x, p4.y);
    SDL_RenderDrawLine(renderer, p4.x, p4.y, p1.x, p1.y);


    while(!(pequal(t1, p4) && pequal(t2, p3))){
      if(!pequal(t1, p4)){
        t1.y++;
      };
      if(!pequal(t2, p3)){
        t2.y++;
      };

      SDL_RenderDrawLine(renderer, t1.x, t1.y, t2.x, t2.y);
    };


  };

  int Column::getScaledWidth() const{
      return bases[0][0] - bases[1][0];

  }
  int Column::getScaledHeight() const{
      return bases[0][1] - bases[3][1];
  }

  bool Column::stopPlayer(float playerX, float playerY){
    if(playerX < bases[0][0] && playerX > bases[1][0] - 50 && playerY < bases[2][1] && playerY > bases[1][1] - 50)
      return true;
    return false;
  };
