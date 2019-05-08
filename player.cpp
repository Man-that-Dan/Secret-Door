#include "player.h"
#include "gamedata.h"
#include "imageFactory.h"
#include "bullets.h"
#include "collisionStrategy.h"
#include "explodingSprite.h"

void Player::advanceFrame(Uint32 ticks) {
	timeSinceLastFrame += ticks;
	if (timeSinceLastFrame > frameInterval && timeSinceLastFrame > 20) {
    currentFrame = (currentFrame+1) % numberOfFrames;
		timeSinceLastFrame = 0;
	}
}

Player::~Player( ) {
  delete collisionStrategy;
  delete explosion;
}

Player::Player( const std::string& name) :
  Drawable(name,
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/startLoc/x"),
                    Gamedata::getInstance().getXmlInt(name+"/startLoc/y")),
           Vector2f(Gamedata::getInstance().getXmlInt(name+"/speedX"),
                    Gamedata::getInstance().getXmlInt(name+"/speedY"))
           ),
  imagesRight( ImageFactory::getInstance()->getImages(name) ),
  imagesLeft( ImageFactory::getInstance()->getImages(name+"Left") ),
	imagesUp( ImageFactory::getInstance()->getImages(name+"Up") ),
	imagesDown( ImageFactory::getInstance()->getImages(name+"Down") ),
  images( imagesRight ),

  currentFrame(0),
  numberOfFrames( Gamedata::getInstance().getXmlInt(name+"/frames") ),
  frameInterval( Gamedata::getInstance().getXmlInt(name+"/frameInterval")),
  timeSinceLastFrame( 0 ),
  worldWidth(Gamedata::getInstance().getXmlInt("world/width")),
  worldHeight(Gamedata::getInstance().getXmlInt("world/height")),
  initialVelocity(getVelocity()),

  collisionStrategy( new PerPixelCollisionStrategy ),
  explosion( nullptr ),

  bulletName(Gamedata::getInstance().getXmlStr(name+"/bulletName")),
  bullets(bulletName),
	sworddown(bulletName+"down"),
	swordleft(bulletName+"left"),
	swordright(bulletName+"right"),
	direction("right"),
	stamina(100),
	health(100),
	immortal(0),
	exploded(false),
  bulletSpeed(Gamedata::getInstance().getXmlInt(bulletName+"/speedX")),
  bulletInterval(Gamedata::getInstance().getXmlInt(bulletName+"/interval")),
  timeSinceLastBullet(0)
{ }

void Player::draw() const {
  if ( explosion ) {
    explosion->draw();
    return;
  }
	if(!exploded){
  	images[currentFrame]->draw(getX(), getY(), getScale());
  	bullets.draw();
		sworddown.draw();
		swordleft.draw();
		swordright.draw();
	};
}

bool Player::shot(const Drawable* obj ) {
  if ( bullets.collidedWith(obj) ||
	  sworddown.collidedWith(obj) ||
	   swordleft.collidedWith(obj)  ||
	swordright.collidedWith(obj)) return true;
  else return false;
}

bool Player::collidedWith(const Drawable* obj ) {
  if ( explosion ) return false;
  if ( collisionStrategy->execute(*this, *obj) ) {
    return true;
  }
  return false;
}

void Player::explode() {
  if ( !explosion ) {
    Sprite
    sprite(getName(), getPosition(), getVelocity(), images[currentFrame]);
    sprite.setScale( getScale() );
    explosion = new ExplodingSprite(sprite);
		exploded = true;
  }
}

void Player::shoot() {
	if(stamina > 0){
	 stamina -= 40;
  // See if it's time to shoot a bullet:
	if (timeSinceLastBullet > bulletInterval) {
    Vector2f vel = getVelocity();
    float x;
    float y = getY()+getScaledHeight()/4+11;
    if ( vel[0] > 0 ) {
      x = getX()+getScaledWidth()-10;
      vel[0] += bulletSpeed;
    }
    else {
      x = getX();
      vel[0] -= bulletSpeed;
    }
		if(direction == "up"){
    	bullets.shoot( Vector2f(x, y - 80), vel );
			timeSinceLastBullet = 0;
		} else if(direction == "right"){
    	swordright.shoot( Vector2f(x + 30, y - 30), vel );
			timeSinceLastBullet = 0;
		} else if(direction == "left"){
    	swordleft.shoot( Vector2f(x - 30, y - 30), vel );
			timeSinceLastBullet = 0;
		} else if(direction == "down"){
    	sworddown.shoot( Vector2f(x, y + 30), vel );
			timeSinceLastBullet = 0;
		};
  }
};
}

void Player::stop() {
  //setVelocity( Vector2f(0, 0) );
  setVelocityX( 0*getVelocityX() );
  setVelocityY(0);
}

void Player::right() {
  if ( getX() < worldWidth-getScaledWidth()) {
    setVelocityX(initialVelocity[0]);
    images = imagesRight;
		direction = "right";
  }
	advanceFrame(3);

}
void Player::left()  {
  if ( getX() > 0) {
    setVelocityX(-initialVelocity[0]);
    images = imagesLeft;
		direction = "left";
  }
	advanceFrame(3);

}
void Player::up()    {
  if ( getY() > 0) {
    setVelocityY( -initialVelocity[1] );
		images = imagesUp;
		direction = "up";
  }
	advanceFrame(3);

}
void Player::down()  {
  if ( getY() < worldHeight-getScaledHeight()) {
    setVelocityY( initialVelocity[1] );
		images = imagesDown;
		direction = "down";

  }
	advanceFrame(3);

}

void Player::update(Uint32 ticks) {
  if ( explosion ) {
    explosion->update(ticks);
    if ( explosion->chunkCount() == 0 ) {
      delete explosion;
      explosion = NULL;
    }
    return;
  }
	if(stamina != 100){
		if(stamina > 99){
			stamina = 100;
		} else {
			stamina += 1;
		};
	};
  //advanceFrame(ticks);
	//if(getVelocityX() > 0 || getVelocityY() > 0){
	//	advanceFrame(1);
	//};
  timeSinceLastBullet += ticks;
  bullets.update(ticks);
	sworddown.update(ticks);
	swordleft.update(ticks);
	swordright.update(ticks);

  Vector2f incr = getVelocity() * static_cast<float>(ticks) * 0.001;
  setPosition(getPosition() + incr);

  if ( getY() < 0) {
    setVelocityY( fabs( getVelocityY() ) );
  }
  if ( getY() > worldHeight-getScaledHeight()) {
    setVelocityY( -fabs( getVelocityY() ) );
  }

  if ( getX() < 0) {
    setVelocityX( fabs( getVelocityX() ) );
    images = imagesRight;
  }
  if ( getX() > worldWidth-getScaledWidth()) {
    setVelocityX( -fabs( getVelocityX() ) );
    images = imagesLeft;
  }

  stop();
}
