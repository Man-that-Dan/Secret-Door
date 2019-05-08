#include <sstream>
#include "viewport.h"
#include "ioMod.h"
#include "renderContext.h"

Viewport& Viewport::getInstance() {
  static Viewport viewport;
  return viewport;
}

Viewport::Viewport() :
  hud(true),
  gdata(Gamedata::getInstance()),
  position(0, 0),
  worldWidth(gdata.getXmlInt("world/width")),
  worldHeight(gdata.getXmlInt("world/height")),
  viewWidth(gdata.getXmlInt("view/width")),
  viewHeight(gdata.getXmlInt("view/height")),
  objWidth(0), objHeight(0),
  objectToTrack(NULL)
{}

void Viewport::toggleHud(){
    hud = !hud;
};

void Viewport::setFree(int freebull){
  std::string status;
  if(freebull == 1){
    status = "on";
  } else {
    status = "off";
  };
  if(hud){
  IoMod::getInstance().
    writeText("Immortal: "+status, 30, 150);
  };
};

void Viewport::Winscreen(){

    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = viewWidth;
    r.h = viewHeight;
    SDL_SetRenderDrawColor( RenderContext::getInstance()->getRenderer(), 0, 10, 30, 255 );
    SDL_RenderFillRect( RenderContext::getInstance()->getRenderer(), &r );
    IoMod::getInstance().
      writeText("You've escaped!",  (viewWidth/2) - 40, viewHeight/2);
    IoMod::getInstance().
      writeText("Q to quit, R to restart",  (viewWidth/2) - 40, (viewHeight/2) + 50);
};

void Viewport::Deadscreen(){

    SDL_Rect r;
    r.x = 0;
    r.y = 0;
    r.w = viewWidth;
    r.h = viewHeight;
    SDL_SetRenderDrawColor( RenderContext::getInstance()->getRenderer(), 150, 30, 30, 255 );
    SDL_RenderFillRect( RenderContext::getInstance()->getRenderer(), &r );
    IoMod::getInstance().
      writeText("You are dead",  (viewWidth/2) - 40, viewHeight/2);
    IoMod::getInstance().
      writeText("Q to quit, R to restart",  (viewWidth/2) - 40, (viewHeight/2) + 50);
};

void Viewport::setHealthandStam(int health, int stamina){
  IoMod::getInstance().
    writeText("Health: ",  10, viewHeight - 40);
    SDL_Rect r;
    r.x = 100;
    r.y = viewHeight - 40;
    r.w = 25;
    r.h = 10;

    IoMod::getInstance().
      writeText("Stamina: ",  400, viewHeight - 40);
    SDL_SetRenderDrawColor( RenderContext::getInstance()->getRenderer(), 255, 0, 0, 255 );
    while(health > 0){
      // Render rect
      SDL_RenderFillRect( RenderContext::getInstance()->getRenderer(), &r );
      r.x += 25;
      health -= 25;
    };
    r.x = 500;
    SDL_SetRenderDrawColor( RenderContext::getInstance()->getRenderer(), 0, 255, 0, 255 );
    while(stamina > 0){
      // Render rect
      SDL_RenderFillRect( RenderContext::getInstance()->getRenderer(), &r );
      r.x += 25;
      stamina -= 25;
    };



};

void Viewport::setObjectToTrack(const Drawable *obj) {
  objectToTrack = obj;
  objWidth = objectToTrack->getScaledWidth();
  objHeight = objectToTrack->getScaledHeight();
}

void Viewport::draw() const {

  if(hud){
    IoMod::getInstance().writeText("use awsd to move, space to attack, hold g to toggle immortality", 30, 30);
    IoMod::getInstance().writeText("Find the clue and solve the puzzle, find the exit to escape", 30, 60);
    IoMod::getInstance().writeText("Interact with clues with T, Press f1 to toggle these messages", 30, 90);
  };
  SDL_Rect r;
  r.x = 0;
  r.y = viewHeight - 70;
  r.w = viewWidth;
  r.h = 70;

 // First set the blend mode so that alpha blending will work;
 // the default blend mode is SDL_BLENDMODE_NONE!

 SDL_SetRenderDrawColor( RenderContext::getInstance()->getRenderer(), 0, 0, 50, 255 );

 // Render rect
 SDL_RenderFillRect( RenderContext::getInstance()->getRenderer(), &r );
}

void Viewport::update() {
  const float x = objectToTrack->getX();
  const float y = objectToTrack->getY();

  position[0] = (x + objWidth/2) - viewWidth/2;
  position[1] = (y + objHeight/2) - viewHeight/2;
  if (position[0] < 0) position[0] = 0;
  if (position[1] < 0) position[1] = 0;
  if (position[0] > (worldWidth - viewWidth)) {
    position[0] = worldWidth-viewWidth;
  }
  if (position[1] > (worldHeight - viewHeight)) {
    position[1] = worldHeight-viewHeight;
  }
}
