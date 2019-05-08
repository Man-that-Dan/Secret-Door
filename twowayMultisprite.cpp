#include "twowayMultisprite.h"
#include "gamedata.h"
#include "imageFactory.h"

#include "collisionStrategy.h"
#include "explodingSprite.h"

void TwowayMultiSprite::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

TwowayMultiSprite::TwowayMultiSprite( const std::string& name, const std::string& direc, float xspeed, float yspeed) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(xspeed, yspeed)
           ),
  imagesRight( ImageFactory::getInstance()->getImages(name) ),
  imagesLeft( ImageFactory::getInstance()->getImages(name+"Left") ),
  images( imagesRight ),
  explosion( nullptr ),
	mode("patrol"),
	boundary(0),
	direction(direc),
	initial_state(Vector2f(xspeed, yspeed)),
	player_loc(Vector2f(0, 0)),
	wall(0),
	safepos(Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
					 Gamedata::getInstance().getXmlInt(name+"/startLoc/y"))),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }

TwowayMultiSprite::TwowayMultiSprite( const std::string& name, const std::string& direc, float x, float y, float xspeed, float yspeed) :
  Drawable(name,
           Vector2f(x, y),
           Vector2f(xspeed, yspeed)
           ),
  imagesRight( ImageFactory::getInstance()->getImages(name) ),
  imagesLeft( ImageFactory::getInstance()->getImages(name+"Left") ),
  images( imagesRight ),
  explosion( nullptr ),
	mode("patrol"),
	boundary(0),
	direction(direc),
	initial_state(Vector2f(xspeed, yspeed)),
	player_loc(Vector2f(0, 0)),
	wall(0),
	safepos(Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
					 Gamedata::getInstance().getXmlInt(name+"/startLoc/y"))),
  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height"))
{ }


void TwowayMultiSprite::explode() {
  if ( !explosion ) {
    Sprite
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    sprite.setScale( getScale() );
    explosion = new ExplodingSprite(sprite);
  }
}

void TwowayMultiSprite::draw() const {
  if ( explosion ) explosion->draw();
  else images[currentFrame]->draw(getX(), getY(), getScale());
}

void TwowayMultiSprite::update(Uint32 ticks) {
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }

	Vector2f mypos = getPosition();
	float diffY = player_loc[1] - mypos[1];
	float diffX = player_loc[0] - mypos[0];
	float sightdist = sqrt((diffY * diffY) + (diffX * diffX));
	if(sightdist < 250){
		mode = "attack";
	} else {
		if(mode == "attack"){
			boundary = 0;
			mode = "return";
		};
	};



	//routine patrol
	if(mode == "patrol"){
		boundary++;
		if(boundary > 500){
			boundary = 0;
			// Vector2f station = Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
			// 				 Gamedata::getInstance().getXmlInt(name+"/startLoc/y"));
			// if(fabs(getPosition().x - station))
			setVelocityY( -getVelocityY());
			setVelocityX( -getVelocityX());
			if(getVelocityX() < 0){
				images = imagesLeft;
			} else {
				images = imagesRight;
			};
		}



	} else if(mode == "attack"){
		Vector2f mypos = getPosition();
		Vector2f newdirection = player_loc - mypos;
		setVelocity(newdirection);

	} else if(mode == "return"){
		if(boundary > 400){
			mode = "reset";
		};
		Vector2f mypos = getPosition();
		Vector2f station = Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
						 Gamedata::getInstance().getXmlInt(name+"/startLoc/y"));
		Vector2f newdirection = station - mypos;
		setVelocity(newdirection);
		boundary++;

	} else if(mode == "reset"){
		setPosition(Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
						 Gamedata::getInstance().getXmlInt(name+"/startLoc/y")));
		setVelocity(initial_state);
		boundary = 0;
		mode = "patrol";
	};

  advanceFrame(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() + getScaledHeight() > worldHeight) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    images = imagesRight;
  }
  if ( getX() + getScaledWidth() > worldWidth) {
    setVelocityX( -fabs( getVelocityX() ) );
    images = imagesLeft;
  }

	if(wall == 1){
		setVelocityX( -getVelocityX());
		setVelocityY( -getVelocityY());
		setPosition(safepos);
		wall = 0;
	}

}
