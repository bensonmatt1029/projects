/**********************************************************************
 * Header File:
 *    PROJECTILE
 * Author:
 *    Matt Benson
 * Summary:
 *    Everything we need to know about a projectile
 ************************************************************************/

#pragma once

#include <list>
#include "position.h"
#include "velocity.h"
#include "physics.h"
#include "uiDraw.h"

#define DEFAULT_PROJECTILE_WEIGHT 46.7       // kg
#define DEFAULT_PROJECTILE_RADIUS 0.077545   // m
#define GRAVITY -9.8064

 // forward declaration for the unit test class
class TestProjectile;

/**********************************************************************
 * Projectile
 * Everything we need to know about a projectile
 ************************************************************************/
class Projectile
{
public:
   // Friend the unit test class
   friend::TestProjectile;

   // create a new projectile with the default settings
   Projectile() : mass(DEFAULT_PROJECTILE_WEIGHT), radius(DEFAULT_PROJECTILE_RADIUS), flightPath() {}

   // reset the game
   void reset()
   {
      flightPath.clear();
      mass = DEFAULT_PROJECTILE_WEIGHT;
      radius = DEFAULT_PROJECTILE_RADIUS;
   }

   // advance the round forward until the next unit of time
   void advance(double simulationTime);

   // getters
   double getAltitude() const { return isFlying() ? flightPath.back().pos.getMetersY() : 0; }
   Position getPosition() const { return isFlying() ? flightPath.back().pos : Position(); }
   double getFlightTime() const
   {
      return (flightPath.size() >= 2) ? flightPath.back().t - flightPath.front().t : 0.0;
   }
   double getFlightDistance() const
   {
      return (flightPath.size() >= 2) ?
         abs(flightPath.front().pos.getMetersX() - flightPath.back().pos.getMetersX()) : 0.0;
   }
   double getSpeed() const { return isFlying() ? flightPath.back().v.getSpeed() : 0.0; }
   double getCurrentTime() const { return isFlying() ? flightPath.back().t : 0.0; }

   // setters
   void setMass(double mass) { this->mass = mass; }
   void setRadius(double radius) { this->radius = radius; }

   // are we flying?
   bool isFlying() const { return !flightPath.empty(); }

   // draw the projectile
   void draw(ogstream& gout) const
   {
      for (auto it = flightPath.cbegin(); it != flightPath.cend(); ++it)
         gout.drawProjectile(it->pos, getCurrentTime() - it->t);
   }

   // fire the projectile
   void fire(const Position& posHowitzer, double simulationTime,
      const Angle& elevation, double muzzleVelocity);

private:

   // keep track of one moment in the path of the projectile
   struct PositionVelocityTime
   {
      PositionVelocityTime() : pos(), v(), t(0.0) {}
      Position pos;
      Velocity v;
      double t;
   };

   double mass;           // weight of the M795 projectile. Defaults to 46.7 kg
   double radius;         // radius of M795 projectile. Defaults to 0.077545 m
   std::list<PositionVelocityTime> flightPath;
};
