#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <random>
#include <cmath>
#include <iomanip>
#include "sprite.h"
#include "multisprite.h"
#include "twowayMultisprite.h"
#include "player.h"
#include "gamedata.h"
#include "engine.h"
#include "frameGenerator.h"
#include "collisionStrategy.h"
#include "column.h"
#include "sound.h"
#include "ioMod.h"
#include "Interactive.h"
#include "viewport.h"

Engine::~Engine() {
  delete enemy;
  delete player;
  std::cout << "Terminating program" << std::endl;
}

Engine::Engine() :
  rc( RenderContext::getInstance() ),
  io( IoMod::getInstance() ),
  clock( Clock::getInstance() ),
  renderer( rc->getRenderer() ),
  viewport( Viewport::getInstance() ),
  farm("farm", 1),
  lights(),
  player(new Player("player")),
  enemy(new TwowayMultiSprite("crow", "x", 0, 150)),
  testCol(getCol()),
  testCol2(getCol2()),
  testCol3(getCol3()),
  testCol4(getCol4()),
  wincol(getDoor()),
  level(),
  theDead(),
  clue(Interactive("clue", Vector2f(950, 30), false, false, "The one who is never beaten doesn't know there is someone better.", 0)),
  water(Interactive("waterPuzzle", Vector2f(3100, 300), true, true, " ", 3)),
  fire(Interactive("firePuzzle", Vector2f(400, 3000), true, true, " ", 6)),
  grass(Interactive("grassPuzzle", Vector2f(3000, 3600), true, true, " ", 0)),
  currentSprite(0),
  open(false),
  won(false),
  dead(false),
  makeVideo( false )
{

  Viewport::getInstance().setObjectToTrack(player);
  makeWalls();
  spawnEnemies();
  std::cout << "Loading complete" << std::endl;
}
//I intend to make these coordinates part of the xml
Column Engine::getCol(){
  Vector2f walls[4];
  walls[0][0] = 750;
  walls[0][1] = 250;
  walls[1][0] = 650;
  walls[1][1] = 250;
  walls[2][0] = 650;
  walls[2][1] = 350;
  walls[3][0] = 750;
  walls[3][1] = 350;
  SDL_Color wallcol = {148, 77, 60, 255};
  Column testCols = Column(walls, wallcol);
  return testCols;
};

Column Engine::getDoor(){
  Vector2f walls[4];
  walls[0][0] = 3600;
  walls[0][1] = 1800;
  walls[1][0] = 3800;
  walls[1][1] = 1800;
  walls[2][0] = 3600;
  walls[2][1] = 2000;
  walls[3][0] = 3800;
  walls[3][1] = 2000;
  SDL_Color wallcol = {148, 77, 60, 255};
  Column testCols = Column(walls, wallcol);
  return testCols;
};

void Engine::makeWalls(){
    unsigned int numberOfWalls = Gamedata::getInstance().getXmlInt("numberOfWalls");
    for(int i = 0; i < numberOfWalls; i++){
      int x = Gamedata::getInstance().getXmlInt("Wall"+std::to_string(i + 1)+"/x");
      int y = Gamedata::getInstance().getXmlInt("Wall"+std::to_string(i + 1)+"/y");
      int height = Gamedata::getInstance().getXmlInt("Wall"+std::to_string(i + 1)+"/height");
      int width = Gamedata::getInstance().getXmlInt("Wall"+std::to_string(i + 1)+"/width");
      Vector2f walls[4];
      walls[0][0] = x + width;
      walls[0][1] = y;
      walls[1][0] = x;
      walls[1][1] = y;
      walls[2][0] = x;
      walls[2][1] = y + height;
      walls[3][0] = x + width;
      walls[3][1] = y + height;
      SDL_Color wallcol = {148, 77, 60, 255};
      Column * testCol = new Column(walls, wallcol);
      level.push_back(testCol);
    }
}

void Engine::spawnEnemies(){
    unsigned int numberOfEnemies = Gamedata::getInstance().getXmlInt("numberOfEnemies");
    for(int i = 0; i < numberOfEnemies; i++){
      int x = Gamedata::getInstance().getXmlInt("Enemy"+std::to_string(i + 1)+"/x");
      int y = Gamedata::getInstance().getXmlInt("Enemy"+std::to_string(i + 1)+"/y");
      int velX = Gamedata::getInstance().getXmlInt("Enemy"+std::to_string(i + 1)+"/velX");
      int velY = Gamedata::getInstance().getXmlInt("Enemy"+std::to_string(i + 1)+"/velY");
      Drawable* wight = new TwowayMultiSprite("crow", "x", x, y, velX, velY);
      theDead.push_back(wight);
    };
}

Column Engine::getCol2(){
  Vector2f walls[4];
  walls[0][0] = 750;
  walls[0][1] = 750;
  walls[1][0] = 650;
  walls[1][1] = 750;
  walls[2][0] = 650;
  walls[2][1] = 850;
  walls[3][0] = 750;
  walls[3][1] = 850;
  SDL_Color wallcol = {148, 77, 60, 255};
  Column testCols = Column(walls, wallcol);
  return testCols;
};

Column Engine::getCol3(){
  Vector2f walls[4];
  walls[0][0] = 1150;
  walls[0][1] = 250;
  walls[1][0] = 1050;
  walls[1][1] = 250;
  walls[2][0] = 1050;
  walls[2][1] = 350;
  walls[3][0] = 1150;
  walls[3][1] = 350;
  SDL_Color wallcol = {148, 77, 60, 255};
  Column testCols = Column(walls, wallcol);
  return testCols;
};

Column Engine::getCol4(){
  Vector2f walls[4];
  walls[0][0] = 1150;
  walls[0][1] = 750;
  walls[1][0] = 1050;
  walls[1][1] = 750;
  walls[2][0] = 1050;
  walls[2][1] = 850;
  walls[3][0] = 1150;
  walls[3][1] = 850;
  SDL_Color wallcol = {148, 77, 60, 255};
  Column testCols = Column(walls, wallcol);
  return testCols;
};

bool Engine::checkInteraction(Vector2f cluepos, Vector2f playerpos){
  if(fabs((cluepos[0] - playerpos[0])) < 40 && fabs((cluepos[1] - playerpos[1])) < 40){
    return true;
  };
  return false;
};

void Engine::draw() const {

  farm.draw();

  enemy->draw();
  std::vector<Drawable*>::const_iterator ptr = theDead.begin();
  while ( ptr != theDead.end() ) {
    (*ptr)->draw();
    ++ptr;
  };

  player->draw();
  std::vector<Column*>::const_iterator ptr2 = level.begin();
  while ( ptr2 != level.end() ) {
    (*ptr2)->draw();
    ++ptr2;
  };
  if(!(clue.takeScreen())){
    clue.draw();
  };
  if(!(water.takeScreen())){
    water.draw();
  };
  if(!(fire.takeScreen())){
    fire.draw();
  };
  if(!(grass.takeScreen())){
    grass.draw();
  };
  testCol.draw();
  testCol2.draw();
  testCol3.draw();
  testCol4.draw();
  //wincol.draw();




  lights.draw();
  viewport.draw();
  int gmode = static_cast<Player*>(player)->isImmortal();
  viewport.setFree(gmode);
  if((clue.takeScreen())){
    clue.draw();
  };
  if((water.takeScreen())){
    water.draw();
  };
  if((fire.takeScreen())){
    fire.draw();
  };
  if((grass.takeScreen())){
    grass.draw();
  };
  if(open == false && player->getPosition()[1] > 1800 && player->getPosition()[1] < 2300 && player->getPosition()[0] > 3800){
    //won = true;
    IoMod::getInstance().writeText("Door locked",  10, 10);
  };
  if(dead){
    viewport.Deadscreen();
  };
  if(won){
    viewport.Winscreen();
  };

  viewport.setHealthandStam(static_cast<Player*>(player)->getHealth(), static_cast<Player*>(player)->getStamina());
  SDL_RenderPresent(renderer);
}

void Engine::update(Uint32 ticks) {
  int wall = 0;
  //enemy collision test
  if(testCol.stopPlayer(enemy->getX(), enemy->getY())){
    wall = 1;
  } else if(testCol2.stopPlayer(enemy->getX(), enemy->getY())){
    wall = 1;
  } else if(testCol3.stopPlayer(enemy->getX(), enemy->getY())){
    wall = 1;
  } else if(testCol4.stopPlayer(enemy->getX(), enemy->getY())){
    wall = 1;
  };
  static_cast<TwowayMultiSprite*>(enemy)->crashed(wall);
  enemy->update(ticks);
  static_cast<TwowayMultiSprite*>(enemy)->inform(player->getPosition());
  std::vector<Drawable*>::const_iterator ptr = theDead.begin();
  while ( ptr != theDead.end() ) {
    wall = 0;
    if(testCol.stopPlayer((*ptr)->getX(), (*ptr)->getY())){
      wall = 1;
    } else if(testCol2.stopPlayer((*ptr)->getX(), (*ptr)->getY())){
      wall = 1;
    } else if(testCol3.stopPlayer((*ptr)->getX(), (*ptr)->getY())){
      wall = 1;
    } else if(testCol4.stopPlayer((*ptr)->getX(), (*ptr)->getY())){
      wall = 1;
    };
    std::vector<Column*>::const_iterator ptr2 = level.begin();
    while ( ptr2 != level.end() ) {
      if((*ptr2)->stopPlayer((*ptr)->getX(), (*ptr)->getY())){
        wall = 1;
      };
      ++ptr2;
    };
    static_cast<TwowayMultiSprite*>((*ptr))->crashed(wall);
    (*ptr)->update(ticks);
    static_cast<TwowayMultiSprite*>((*ptr))->inform(player->getPosition());
    ++ptr;
  };
  player->update(ticks);
  lights.update();
  clue.update(ticks);
  water.update(ticks);
  fire.update(ticks);
  grass.update(ticks);
  if(water.isSolved() && fire.isSolved() && grass.isSolved()){

    open = true;

  };
  if(open == true && player->getPosition()[1] > 1800 && player->getPosition()[1] < 2300 && player->getPosition()[0] > 3800){
    won = true;
    //IoMod::getInstance().writeText("win ",  10, 10);
  };
  farm.update();
  viewport.update(); // always update viewport last
}

void Engine::checkForCollisions() {
  std::vector<Drawable*>::const_iterator ptr = theDead.begin();
  while ( ptr != theDead.end() ) {
    if ( player->collidedWith((*ptr))) {
      if(static_cast<Player*>(player)->dead() == 1 && static_cast<Player*>(player)->isImmortal() == 0){
        static_cast<Player*>(player)->explode();
      } else {
        static_cast<Player*>(player)->hurt();
        static_cast<TwowayMultiSprite*>((*ptr))->crashed(1);
      };
      //enemy->explode();
    }
    if ( player->shot((*ptr)) ) {
      (*ptr)->explode();
    }

    ++ptr;
  };
  if ( player->collidedWith(enemy)) {
    if(static_cast<Player*>(player)->dead() == 1 && static_cast<Player*>(player)->isImmortal() == 0){
      static_cast<Player*>(player)->explode();
      dead = true;
    } else {
      static_cast<Player*>(player)->hurt();
      static_cast<TwowayMultiSprite*>(enemy)->crashed(1);
    };
    //enemy->explode();
  }
  if ( player->shot(enemy) ) {
    enemy->explode();
  }
}

void Engine::switchSprite(){
  ++currentSprite;
  currentSprite = currentSprite % 2;
  if ( currentSprite ) {
    Viewport::getInstance().setObjectToTrack(player);
  }
  else {
    Viewport::getInstance().setObjectToTrack(enemy);
  }
}

int Engine::play() {
  SDLSound sound;
  bool restart = false;
  int timesincetoggle = 0;
  bool hud = true;
  player->setScale(3);
  Interactive* active_screen = &clue;
  SDL_Event event;
  const Uint8* keystate;
  float safeX, safeY;
  int stopped = 0;
  bool done = false;
  Uint32 ticks = clock.getElapsedTicks();
  FrameGenerator frameGen;

  while ( !done ) {
    // The next loop polls for events, guarding against key bounce:

    while ( SDL_PollEvent(&event) ) {
      keystate = SDL_GetKeyboardState(NULL);

      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDL_SCANCODE_ESCAPE] || keystate[SDL_SCANCODE_Q]) {
          done = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_P] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if ( keystate[SDL_SCANCODE_R] ) {
          done = true;
          restart = true;
          break;
        }
        if ( keystate[SDL_SCANCODE_G] ) {
          if(timesincetoggle == 0){
            static_cast<Player*>(player)->toggleGmode();
            timesincetoggle = 10;
            std::cout << "toggle\n";
          } else {
            timesincetoggle--;
          };
        }
        if ( keystate[SDL_SCANCODE_F1] ) {
          viewport.toggleHud();
        }

        if (keystate[SDL_SCANCODE_F4] && !makeVideo) {
          std::cout << "Initiating frame capture" << std::endl;
          makeVideo = true;
        }
        else if (keystate[SDL_SCANCODE_F4] && makeVideo) {
          std::cout << "Terminating frame capture" << std::endl;
          makeVideo = false;
        }
      }
    }
    // I realize I should be using getters and setters but its late, and im tired.
    // Not thinking straight, I'll fix it for the final submission


    // In this section of the event loop we allow key bounce:
    ticks = clock.getElapsedTicks();
    if ( ticks > 0 ) {

      clock.incrFrame();

      if(!(active_screen->takeScreen())){
      if (keystate[SDL_SCANCODE_SPACE]) {
        static_cast<Player*>(player)->shoot();
        sound[4];
      }
      if (keystate[SDL_SCANCODE_A]) {
        static_cast<Player*>(player)->left();
        //clue.turnLeft();
      }
      if (keystate[SDL_SCANCODE_D]) {
        static_cast<Player*>(player)->right();
        //clue.turnRight();
      }
      if (keystate[SDL_SCANCODE_W]) {
        static_cast<Player*>(player)->up();
      }
      if (keystate[SDL_SCANCODE_S]) {
        static_cast<Player*>(player)->down();
      }

      if ( keystate[SDL_SCANCODE_T] ) {
        if(checkInteraction(static_cast<Drawable*>(&clue)->getPosition(), player->getPosition())){
          active_screen = &clue;
          active_screen->toggle();
        };
        if(checkInteraction(static_cast<Drawable*>(&water)->getPosition(), player->getPosition())){
          active_screen = &water;
          active_screen->toggle();
        };
        if(checkInteraction(static_cast<Drawable*>(&fire)->getPosition(), player->getPosition())){
          active_screen = &fire;
          active_screen->toggle();
        };
        if(checkInteraction(static_cast<Drawable*>(&grass)->getPosition(), player->getPosition())){
          active_screen = &grass;
          active_screen->toggle();
        };

      }
    } else {
      if (keystate[SDL_SCANCODE_SPACE]) {
        active_screen->toggle();

      }
      if (keystate[SDL_SCANCODE_A]) {

        active_screen->turnLeft();
      }
      if (keystate[SDL_SCANCODE_D]) {

        active_screen->turnRight();
      }

    };
    std::vector<Column*>::const_iterator ptr = level.begin();
    while ( ptr != level.end() ) {
      (*ptr)->update();
      ++ptr;
    };


    bool crashed = false;
    ptr = level.begin();
    while ( ptr != level.end() ) {
      if((*ptr)->stopPlayer(player->getX(), player->getY())){
        crashed = true;
      };
      ++ptr;
    };
      //make this into an iterator
      testCol.update();
      testCol2.update();
      testCol3.update();
      testCol4.update();
      //wincol.update();
      if(testCol.stopPlayer(player->getX(), player->getY())){
        crashed = true;
      } else if(testCol2.stopPlayer(player->getX(), player->getY())){
        crashed = true;

      } else if(testCol3.stopPlayer(player->getX(), player->getY())){
        crashed = true;

      } else if(testCol4.stopPlayer(player->getX(), player->getY())){
        crashed = true;

      } else if(wincol.stopPlayer(player->getX(), player->getY())){
        if(!open){
          crashed = true;
        };
      };

      if(crashed){
        player->setX(safeX);
        player->setY(safeY);
      } else {
        safeX = player->getX();
        safeY = player->getY();
      }
      checkForCollisions();
      draw();



      update(ticks);
      if ( makeVideo ) {
        frameGen.makeFrame();
      }
    }
  }
  if(restart){
    return 1;
  } else {
    return 0;
  };
}
