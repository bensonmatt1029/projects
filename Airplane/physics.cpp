/***********************************************************************
 * Source File:
 *    PHYSICS
 * Author:
 *    Matt Benson
 * Summary:
 *    Laws of motion, effects of gravity, wind resistence, etc.
 ************************************************************************/

#include "physics.h"  // for the prototypes

const double earthRadius = 6378000.0;

/**********************************************************
 * GET ALTITUDE
 * Return the altitude of a point above the earth's surface
 *********************************************************/
double getAltitude(const Position& posElement)
{
   double distance = computeDistance(Position(), posElement);
   return distance - earthRadius;
}

/**********************************************************
 * ACCELERATION GET GRAVITY
 * Equation: a = g_0 (R_e / (r_e + h)) ^ 2
 *********************************************************/
Acceleration getGravity(const Position& posElement)
{
   // compute the hight above the earth
   double height = getAltitude(posElement);

   // direction of gravity 
   Angle angle;
   angle.setDxDy(-posElement.getMetersX(), -posElement.getMetersY());

   double standardGravity = 9.806;
   double tmp = earthRadius / (earthRadius + height);
   double acceleration = standardGravity * tmp * tmp;

   return Acceleration(acceleration, angle);
}

/**********************************************************
* VELOCITY UPDATE VELOCITY
* Update the current velocity
*********************************************************/
Velocity& updateVelocity(Velocity& velocity, const Acceleration& acceleration, double time)
{
   velocity.addDX(acceleration.getDDX() * time);
   velocity.addDY(acceleration.getDDY() * time);
   return velocity;
}

/**********************************************************
* POSITION UPDATE POSITION
* Update the current position
*********************************************************/
Position& updatePosition(Position& pos, const Velocity& vel, const Acceleration& acceleration, double time)
{
   pos.addMetersX((vel.getDX() * time) + ((0.5 * acceleration.getDDX()) * (time * time)));
   pos.addMetersY((vel.getDY() * time) + ((0.5 * acceleration.getDDY()) * (time * time)));
   return pos;
}
