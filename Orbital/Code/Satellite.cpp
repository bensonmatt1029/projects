/***********************************************************************
 * Source File:
 *    Satellite
 * Author:
 *    Matt Benson
 * Summary:
 *    Everything we need to know about a satellite
 ************************************************************************/

#include "satellite.h"
#include "ship.h"
#include "starlink.h"
#include "sputnik.h"
#include "hubble.h"
#include "crewDragon.h"
#include "gps.h"

 /************************************
 * SATELLITE
 * Create a Satellite
 ************************************/
Satellite::Satellite(const Satellite& parent, const Angle& angle) :
   velocity(parent.velocity),
   pos(parent.pos),
   angle(parent.angle),
   angularVelocity(parent.angularVelocity),
   radius(0.0),
   age(0),
#ifndef NDEBUG
   useRandom(parent.useRandom),
#endif // DEBUG
   dead(false)
{
   // compute the kick
   double speed = random(1000.0, 3000.0);
#ifndef NDEBUG
   if (parent.useRandom == false)
      speed = 3000.0;
#endif // DEBUG
   Velocity kick(speed, angle);
   velocity += kick;

   // compute the offset
   Position posKick;
   posKick.setPixelsX(4.0 * sin(angle.getRadians()));
   posKick.setPixelsY(4.0 * cos(angle.getRadians()));
   pos.addMetersX(posKick.getMetersX());
   pos.addMetersY(posKick.getMetersY());
}

/************************************
 * SATELLITE
 * Create a Satellite with an advance
 ************************************/
Satellite::Satellite(const Satellite& parent, const Position& offset, const Velocity& kick) :
   velocity(parent.velocity),
   pos(parent.pos),
   angle(parent.angle),
   angularVelocity(parent.angularVelocity),
   radius(0.0),
   age(0),
#ifndef NDEBUG
   useRandom(parent.useRandom),
#endif // DEBUG
   dead(false)

   // new velocity is the velocity of the parent plus the angle of the "kick"
{
   velocity += kick;

   // advance by one time unit
   pos.addMetersX(offset.getMetersX());
   pos.addMetersY(offset.getMetersY());
}

/************************************
 * SATELLITE :: MOVE
 * Tracks the movement of the satellite
 ************************************/
void Satellite::move(double time)
{

   // gravity and intertia
   Acceleration aGravity = getGravity(pos);

   // update velocity and position due to gravity
   this->velocity.add(aGravity, time / 2.0);
   this->pos.add(aGravity, velocity, time);
   this->velocity.add(aGravity, time / 2.0);
   this->angle.add(angularVelocity);
   this->age++;
}

/************************************
 * Projectile :: Projectile
 * Initializes the projectile
 ************************************/
Projectile::Projectile(const Ship& parent, Velocity bullet)
{
   this->pos = parent.getPos();
   this->angle = Angle(getAngle());
   this->velocity = bullet + parent.getVel();
   angularVelocity = 0.0;
   dead = false;
   this->radius = 1.0 * pos.getZoom();
   age = 0;
}

/************************************
 * Projectile :: MOVE
 * Tracks the movement of the projectile
 ************************************/
void Projectile::move(double timeDilation)
{
   Satellite::move(timeDilation);
   if (this->age > 100)
      this->dead = true;
}

/************************************
 * Fragments :: MOVE
 * Fragments die
 ************************************/
void Fragment::move(double timeDilation)
{
   Satellite::move(timeDilation);
   if (this->age > 100)
      this->dead = true;
}

/************************************
 * WHOLE :: MOVE
 * Whole satellites move
 ************************************/
void Whole::move(double timeDilation)
{
   
   Satellite::move(timeDilation);
   if (random(0, this->chanceDefunct) == 0)
   {
      this->defunct = true;
      this->angularVelocity = -0.08;
   }
}

/************************************
 * Factory
 ************************************/
Satellite* factory(SatellitesType st,
   const Satellite& satellite,
   const Angle& angle)
{
   switch (st)
   {
   case FRAGMENT:
      return new Fragment(satellite, angle);
   case GPS_WHOLE:
      return new GPS((int)angle.getDegrees() / 360);
   case GPS_LEFT:
      return new GPSLeft(satellite, angle);
   case GPS_RIGHT:
      return new GPSRight(satellite, angle);
   case GPS_CENTER:
      return new GPSCenter(satellite, angle);
   case SHIP:
      return new Ship(); 
   case HUBBLE:
      return new Hubble();
   case HUBBLE_COMPUTER:
      return new HubbleComputer(satellite, angle);
   case HUBBLE_LEFT:
      return new HubbleLeft(satellite, angle);
   case HUBBLE_RIGHT:
      return new HubbleRight(satellite, angle);
   case CREWDRAGON_CENTER:
      return new DragonCenter(satellite, angle);
   case CREWDRAGON_LEFT:
      return new DragonLeft(satellite, angle);
   case CREWDRAGON_RIGHT:
      return new DragonRight(satellite, angle);
   case SPUTNIK:
      return new Sputnik();
   case STARLINK_ARRAY:
      return new StarlinkArray(satellite, angle);
   case STARLINK_BODY:
      return new StarlinkBody(satellite, angle);
   }
}

