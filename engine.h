#include <vector>
#include <SDL.h>
#include "ioMod.h"
#include "renderContext.h"
#include "clock.h"
#include "world.h"
#include "viewport.h"
#include "column.h"
#include "lights.h"
#include "Interactive.h"

class Player;

class Engine {
public:
  Engine ();
  ~Engine ();
  int play();
  void switchSprite();

private:
  const RenderContext* rc;
  const IoMod& io;
  Clock& clock;

  SDL_Renderer * const renderer;

  World farm;
  Lights lights;
  Viewport& viewport;

  Drawable* player;
  Drawable* enemy;
  Column testCol;
  Column testCol2;
  Column testCol3;
  Column testCol4;
  Column wincol;
  std::vector<Column*> level;
  std::vector<Drawable*> theDead;
  Interactive clue;
  Interactive water;
  Interactive fire;
  Interactive grass;
  int currentSprite;
  bool open;
  bool won;
  bool dead;
  Column getCol();
  Column getCol2();
  Column getCol3();
  Column getCol4();
  Column getDoor();
  bool makeVideo;
  bool checkInteraction(Vector2f cluepos, Vector2f playerpos);
  void draw() const;
  void update(Uint32);

  Engine(const Engine&);
  Engine& operator=(const Engine&);
  void printScales() const;
  void checkForCollisions();
  void makeWalls();

  void spawnEnemies();
};
