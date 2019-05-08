#include <SDL.h>
#include <iostream>
#include <string>
#include "Interactive.h"
#include "gamedata.h"
#include "imageFactory.h"
#include "image.h"
#include "renderContext.h"
#include "ioMod.h"
#include "viewport.h"



Interactive::Interactive(const std::string& n, const Vector2f& pos, bool hasImage, bool isPuzzle, std::string messg, int solution) :
  Drawable(n, pos, Vector2f(0, 0)),
  image(ImageFactory::getInstance()->getImage("Interactive_spot")),
  images(),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  viewHeight(Gamedata::getInstance().getXmlInt("view/height")),
  viewWidth(Gamedata::getInstance().getXmlInt("view/width")),
  imageIncluded(hasImage),
  puzzle(isPuzzle),
  solved(false),
  active(false),
  message(messg),
  currentFrame(0),
  solvestate(solution)
   {
     if(hasImage){
       images = ImageFactory::getInstance()->getImages(n);
     };
   }

     //note this needs to be drawn last but before HUD to work
  void Interactive::draw() const {
    if(!active){

      image->draw(getX(), getY(), getScale());
    } else {
      SDL_Rect r;
      r.x = 0;
      r.y = 0;
      r.w = viewWidth;
      r.h = viewHeight;
      SDL_SetRenderDrawColor( RenderContext::getInstance()->getRenderer(), 0, 0, 20, 200 );
      SDL_RenderFillRect( RenderContext::getInstance()->getRenderer(), &r );
      IoMod::getInstance().
        writeText("Space to exit",  20, 20);
      IoMod::getInstance().
        writeText(message,  20, 300);
      if(imageIncluded){
        images[currentFrame]->draw(Viewport::getInstance().getX() + viewWidth/2, Viewport::getInstance().getY() + viewHeight/2, 15.0);
      };
    };
  };

  void Interactive::turnLeft(){
    if(currentFrame != 0 && currentFrame){
      currentFrame = (currentFrame - 1 % 8);
    } else {
      currentFrame = 7;
    }
    if(currentFrame == 8){
      currentFrame = 0;
    };
    std::cout << "frame " << currentFrame << std::endl;
  };

  void Interactive::turnRight(){
    currentFrame = (currentFrame + 1 % 8);
    if(currentFrame == 8){
      currentFrame = 0;
    };
    std::cout << "frame " << currentFrame << std::endl;
  };

  void Interactive::update(Uint32 ticks) {
    if(currentFrame == solvestate){
      solved = true;
    } else {
      solved = false;
    };
    std::cout << "solved " << solved << std::endl;
  };

  void Interactive::toggle(){
    active = !active;
  };
