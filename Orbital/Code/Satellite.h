/***********************************************************************
 * Header File:
 *    Satellite
 * Author:
 *    Matt Benson
 * Summary:
 *    Everything we need to know about a satellite
 ************************************************************************/

#pragma once

#include "velocity.h"
#include "position.h"
#include "angle.h"
#include "acceleration.h"
#include "uiInteract.h"
#include "uiDraw.h"
#include "physics.h"
#include "thrust.h"
#include <list>

class TestSatellite;
class Demo;

enum SatellitesType
{
   SHIP,
   GPS_WHOLE, GPS_LEFT, GPS_RIGHT, GPS_CENTER,
   HUBBLE, HUBBLE_LEFT, HUBBLE_RIGHT, HUBBLE_COMPUTER, HUBBLE_CENTER,
   STARLINK, STARLINK_BODY, STARLINK_ARRAY,
   SPUTNIK,
   CREWDRAGON_LEFT, CREWDRAGON_RIGHT, CREWDRAGON_CENTER,
   FRAGMENT
};

/************************************
 * Satellite
 ************************************/
class Satellite
{
public:
   friend TestSatellite;
   friend Demo;

   //
   // Constructors
   //

   // default constructor
   Satellite(int age = 0, double radius = 0.0, double angularVelocity = 0.0)
      : angularVelocity(angularVelocity), dead(false), age(age), radius(radius)
   {
#ifndef NDEBUG
      useRandom = true;
#endif // DEBUG
   }


   // creates parts and fragments
   Satellite(const Satellite& parent, const Angle& angle);

   // creates projectiles
   Satellite(const Satellite& parent, const Position& offset, const Velocity& kick);

   virtual ~Satellite() {}

   //
   // Getters
   //

   // return the radius in meters
   double getRadius() const {return radius; }

   // are we dead
   bool isDead() const { return dead; }

   // this has got to go
   bool isInvisible() const { return age < 10; }

   // where are we located
   const Position& getPosition() const { return pos; }

   // where are we pointed
   const Angle& getAngle() const { return angle; }

   // velocity of satellite
   const Velocity& getVelocity() const { return velocity; }

   // 
   // Setters
   //

   void kill() { if (!isInvisible()) dead = true; }

   //
   // Overridden
   //

   virtual bool getDefunct() { return false; }

   // draw the item
   virtual void draw(ogstream & gout) {}

   // kill the element
   virtual void destroy(std::list <Satellite*>& satellites) {}

   // advance the item by seconds (timeDilation)
   virtual void move(double time);

   // handle input
   virtual void input(const Interface& ui, std::list <Satellite*>& satellites) {}

protected:
   Velocity velocity;
   Position pos;
   Angle angle;
   double angularVelocity;
   bool dead;
   double radius;
   int age;
#ifndef NDEBUG
   bool useRandom;
#endif // DEBUG
};

class Ship;

/****************************************
* PROJECTILE
* Shoots out of the ship
****************************************/
class Projectile : public Satellite
{
public:
   Projectile(const Ship& parent, Velocity bullet);

   void move(double timeDilation);

   bool getDefunct() const { return true; }

   virtual void draw(ogstream& gout)
   {
      gout.drawProjectile(this->pos);
   }

};

/****************************************
* FRAGMENT
* Piece of the satellite
****************************************/
class Fragment : public Satellite
{
public:
   Fragment(const Satellite& parent, const Angle& angle) : Satellite(parent, angle)
   {
      this->age = random(0, 50);
      this->radius = 2.0 * this->pos.getZoom();
   }

   bool getDefunct() const { return true; }

   void move(double timeDilation);

   virtual void draw(ogstream& gout)
   {
      gout.drawFragment(this->pos, this->angle.getRadians());
   }

};

/****************************************
* WHOLE SATELLITE
* The whole satellite
****************************************/
class Whole : public Satellite
{
public:
   // create a part from a whole
   Whole(int chanceDefunct) : chanceDefunct(chanceDefunct), defunct(false), Satellite() {}

   virtual ~Whole() {}

   // maintain whether we are defunct
   bool getDefunct() const { return defunct; }
   void setDefunct(bool defunct) { this->defunct = defunct; }

   // whole satellites can go defunct at any time
   void move(double timeDilation);

protected:
   bool defunct;
   int chanceDefunct;
};

/****************************************
* PART
* A big chunk of a satellite
****************************************/
class Part : public Satellite
{
public:
   Part(const Satellite& parent, const Angle& angle) : Satellite(parent, angle) {}

   // parts are always defunct
   bool getDefunct() const { return true; }
};


/****************************************
* FACTORY
* 
****************************************/
Satellite* factory(SatellitesType st,
                   const Satellite& satellite,
                   const Angle& angle);
